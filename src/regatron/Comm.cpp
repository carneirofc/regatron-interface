
#include "Comm.hpp"
#include <optional>

namespace Regatron {

Comm::Comm(int port)
    : m_Port(port), m_PortNrFound(-1), m_CommStatus{CommStatus::Disconncted},
      m_readings(std::make_shared<Regatron::Readings>()), m_Connected(false),
      m_AutoReconnect(true),
      m_AutoReconnectAttemptTime(std::chrono::system_clock::now()),
      m_AutoReconnectInterval(std::chrono::seconds{15}) {}

Comm::Comm() : Comm(1) {}

Comm::~Comm() {
    disconnect();
    LOG_DEBUG("Comm object destroyed!");
}

void Comm::ReadCommStatus() {
	int pState{-1};
	int pErrorNo{0};

	if (DllGetStatus(&pState, &pErrorNo) != DLL_SUCCESS) {
		LOG_WARN(R"(DLL: Failed to get DLL status. CommStatus set to "Disconnected")");
		m_CommStatus = CommStatus::Disconncted;
	}

	switch (pState) {
	case DLL_STATUS_OK:
		m_CommStatus = CommStatus::Ok;
		break;

	case DLL_STATUS_COMMUNICATION_ERROR:
		LOG_WARN(R"(DLL Communication Error. State="{}", ErrorNo="{}")",
				 pState, pErrorNo);
		m_CommStatus = CommStatus::DLLCommunicationFail;
		break;

	case DLL_STATUS_COMMAND_EXECUTION_ERROR:
		LOG_WARN(R"(DLL Command Execution Error. State="{}", ErrorNo="{}")",
				 pState, pErrorNo);
		m_CommStatus = CommStatus::DLLCommandExecutionFail;
		break;
	default:
        // We should never get to this point...
		LOG_WARN(R"(Unknown DLL Status. State="{}", ErrorNo="{}")", pState,
				 pErrorNo);
		m_CommStatus = CommStatus::Disconncted;
	}
}

void       Comm::SetAutoReconnectInterval(std::chrono::seconds &&seconds) {
    m_AutoReconnectInterval = seconds;
}
std::chrono::seconds Comm::GetAutoReconnectInterval() const {
    return m_AutoReconnectInterval;
}

void Comm::disconnect() {
    auto result  = DllClose();
    m_Connected  = false;
    m_CommStatus = CommStatus::Disconncted;

    /** Reset DLL Variables */
    // Connection
    m_PortNrFound = -1;

    m_readings->Reset();

    LOG_WARN(
        R"(Dllclose: Driver/Objects used by the TCIO are closed, released memory (code "{}"))",
        result);
}

std::optional<std::shared_ptr<Regatron::Readings>> Comm::getReadings() {
    if (m_CommStatus != CommStatus::Ok) {
        LOG_ERROR(R"(Invalid DLL communication status "{}")",
                  static_cast<int>(m_CommStatus));
        return {};
    }
    return {m_readings};
}

void Comm::autoConnect() {
    if (m_Connected && m_CommStatus != CommStatus::Ok) {
        LOG_WARN(R"(Inconsistency detected, m_Connected is "true" but CommStatus is not "Ok",)"
            " probably the power supply has been turned off. Forcing a communication restart..."
        );
        disconnect();
    }

    if (m_AutoReconnect && !m_Connected) {
        const auto now       = std::chrono::system_clock::now();
        const auto timeDelta = now - m_AutoReconnectAttemptTime;

        if ((timeDelta < m_AutoReconnectInterval) && !m_InitialConnection) {
            LOG_TRACE(
                R"(autoconnect: timeout will be active for more "{} s", ignoring attempt.)",
                (std::chrono::duration_cast<std::chrono::seconds>(
                     m_AutoReconnectInterval - timeDelta))
                    .count());
            return;
        }

        LOG_TRACE(R"(autoconnect: attempting to connect after "{} s".)",
                  std::chrono::duration_cast<std::chrono::seconds>(timeDelta)
                      .count());
        m_AutoReconnectAttemptTime = now;
        try {
            m_InitialConnection = false;
            connect();
        } catch (const CommException &e) {
            LOG_ERROR(R"(autoconnect: Failed to connect "{}")", e.what());
        }
    }
}

CommStatus Comm::getCommStatus() const {
    return m_CommStatus;
}

bool       Comm::getAutoReconnect() const {
    return m_AutoReconnect;
}

void       Comm::setAutoReconnect(bool autoReconnect) {
    m_AutoReconnect = autoReconnect;
    LOG_TRACE(R"(autoReconnect: "{}")", m_AutoReconnect);
}

void Comm::InitializeDLL() {
    LOG_TRACE("Initializing TCIO lib.");
    if (DllInit() != DLL_SUCCESS) {
        throw CommException("Failed to initialize TCIO lib.");
    }
    ReadCommStatus();
    if (m_CommStatus != CommStatus::Ok) {
        throw CommException("dll status: Invalid return status.", m_CommStatus);
    }
    m_readings->getVersion().ReadDllVersion();
}
bool Comm::connect() { return connect(m_Port, m_Port); }
bool Comm::connect(int port) { return connect(port, port); }
bool Comm::connect(int fromPort, int toPort) {
    if (m_Connected) {
        LOG_WARN(
            R"(Already connected to a device, consider using "cmdDisconnect")");
        return false;
    }

    if (fromPort < 0 || toPort < 0 || fromPort > toPort) {
        throw CommException(
            fmt::format(R"(invalid port range [{},{}].)", fromPort, toPort));
    }

    InitializeDLL();
#if __linux__
    if (DllSetSearchDevice2ttyDIGI() != DLL_SUCCESS) {
        throw CommException("failed to set ttyDIGI string pattern.");
    }
#endif

    if (fromPort == toPort) {
        LOG_INFO(R"(searching DIGI RealPort device "{}{:02}")", DEVICE_PREFIX,
                 fromPort);
    } else {
        LOG_INFO(
            R"(searching DIGI RealPort device in range "{}{:02}" to "{}{:02}")",
            DEVICE_PREFIX, fromPort, DEVICE_PREFIX, toPort);
    }

    // use this function for VM or rs232 over ethernet
    unsigned int readTout{0};
    unsigned int writeTout{0};
    if (DllSetCommTimeouts(READ_TIMEOUT_MULTIPLIER, WRITE_TIMEOUT_MULTIPLIER) !=
        DLL_SUCCESS) {
        throw CommException(R"("Failed to set DLL comm timeouts.")");
    }

    if (DllGetCommTimeouts(&readTout, &writeTout) != DLL_SUCCESS) {
        throw CommException(R"("Failed to get actual DLL comm timeouts.")");
    }
    LOG_TRACE(R"(Timeout after configuration: "read={}" "write={}".)",
              readTout, writeTout);

    // hack: while eth and rs232 at the same tc device: wait 2 sec
    std::this_thread::sleep_for(DELAY_RS232);

    m_PortNrFound = -1; // Zero m_PortNrFound
#if __linux__
    if (DllSearchDevice(fromPort + 1, toPort + 1, &m_PortNrFound) !=
#else
    if (DllSearchDevice(fromPort, toPort, &m_PortNrFound) !=
#endif
            DLL_SUCCESS ||
        m_PortNrFound == -1) {
        throw CommException(fmt::format(
            R"(Failed to connect to a device in range "{}{:02}" to "{}{:02}" (pPortNrFound={}).)",
            DEVICE_PREFIX, fromPort, DEVICE_PREFIX, toPort, m_PortNrFound));
    }

#if __linux__
    LOG_TRACE(R"(Connected to device number "{}" at "{}{:02}.")", m_PortNrFound,
              DEVICE_PREFIX, m_PortNrFound - 1);
#else
    LOG_TRACE(R"(Connected to device number "{}" at "{}{:02}.")", m_PortNrFound,
              DEVICE_PREFIX, m_PortNrFound);
#endif
    m_Connected = true;

    int pActBaudRate{0};
    if (DllGetCommBaudrate(&pActBaudRate) != DLL_SUCCESS) {
        throw CommException("Failed read baudrate");
    }
    LOG_INFO("Baudrate: {}.", pActBaudRate);

    // set remote control to RS232
    if (TC4SetRemoteControlInput(2) != DLL_SUCCESS) {
        throw CommException("failed to set remote control do RS232.");
    }
    LOG_TRACE("Remote control set to RS232.");

    m_readings->Initialize();

    LOG_INFO(
        R"(Regatron device connected at port "{}", configured as "{}" with module ID "{}".)",
        m_PortNrFound, ((m_readings->isMaster()) ? "master" : "slave"),
        m_readings->getModuleID());

    m_CommStatus = CommStatus::Ok;
    return true;
}
} // namespace Regatron
