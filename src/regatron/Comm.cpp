
#include "Comm.hpp"
#include <optional>

namespace Regatron {

Comm::Comm(int port)
    : m_Port(port), m_readings(std::make_shared<Regatron::Readings>()),
      m_CommStatus{CommStatus::Disconncted}, m_AutoReconnect(true),
      m_Connected(false) {
}

Comm::Comm() : Comm(1) {}

Comm::~Comm() {
    disconnect();
    LOG_TRACE("Comm destroyed!");
}

void Comm::disconnect() {
    auto result = DllClose();
    m_Connected = false;
    m_CommStatus = CommStatus::Disconncted;
    LOG_WARN(
        R"(Dllclose: Driver/Objects used by the TCIO are closed, released memory (code "{}"))",
        result);
}

std::optional<std::shared_ptr<Regatron::Readings>> Comm::getReadings() {
    if (m_CommStatus != CommStatus::Ok) {
        LOG_ERROR(R"(Invalid DC-Link communication status "{}")",
                  static_cast<int>(m_CommStatus));
        return {};
    }
    return {m_readings};
}

void Comm::autoConnect() {
    if (m_AutoReconnect && !m_Connected) {
        auto now       = std::chrono::system_clock::now();
        auto timeDelta = now - m_AutoReconnectAttemptTime;

        if (timeDelta < AUTOCONNECT_INTERVAL) {
            LOG_TRACE(
                R"(autoconnect: timeout is active for more "{} s", ignoring atempt.)",
                (std::chrono::duration_cast<std::chrono::seconds>(
                     AUTOCONNECT_INTERVAL - timeDelta))
                    .count());
            return;
        }

        LOG_TRACE(R"(autoconnect: attempting to connect after "{} s".)",
                  std::chrono::duration_cast<std::chrono::seconds>(timeDelta)
                      .count());
        try {
            m_AutoReconnectAttemptTime = now;
            connect();
        } catch (const CommException &e) {
            LOG_ERROR(R"(autoconnect: Failed to connect "{}")", e.what());
        }
    }
}

CommStatus  Comm::getCommStatus() const { return m_CommStatus; }
bool        Comm::getAutoReconnect() const { return m_AutoReconnect; }
void        Comm::setAutoReconnect(bool autoReconnect) {
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
    m_readings->getVersion()->ReadDllVersion();
}
bool Comm::connect() { return connect(m_Port, m_Port); }
bool Comm::connect(int port) { return connect(port, port); }
bool Comm::connect(int fromPort, int toPort) {
    if (m_Connected) {
        LOG_WARN(R"(Already connected to a device, consider using "cmdDisconnect")");
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
       LOG_INFO(R"(searching DIGI RealPort device "{}{:02}")",
               DEVICE_PREFIX,
               fromPort);
    } else {
       LOG_INFO(
           R"(searching DIGI RealPort device in range "{}{:02}" to "{}{:02}")",
           DEVICE_PREFIX,fromPort,DEVICE_PREFIX,toPort);
    }

    // use this function for VM or rs232 over ethernet
    unsigned int readTout{0};
    unsigned int writeTout{0};
    if (DllSetCommTimeouts(READ_TIMEOUT_MULTIPLIER, WRITE_TIMEOUT_MULTIPLIER) !=
        DLL_SUCCESS) {
        throw CommException(R"("Failed to set DLL comm timeouts.")");
    }

    if(DllGetCommTimeouts(&readTout, &writeTout) != DLL_SUCCESS){
        throw CommException(R"("Failed to get actual DLL comm timeouts.")");
    }
    LOG_TRACE(R"(Timeoute after configuration: "read={}" "write={}".)", readTout, writeTout);

    // hack: while eth and rs232 at the same tc device: wait 2 sec
    std::this_thread::sleep_for(DELAY_RS232);

    m_PortNrFound = -1; // Zero m_PortNrFound
#if __linux__
    if (DllSearchDevice(fromPort + 1, toPort + 1, &m_PortNrFound) !=
#else
    if (DllSearchDevice(fromPort, toPort, &m_PortNrFound) !=
#endif
           DLL_SUCCESS || m_PortNrFound == -1) {
       throw CommException(fmt::format(
           R"(Failed to connect to a device in range "{}{:02}" to "{}{:02}" (pPortNrFound={}).)",
           DEVICE_PREFIX, fromPort, DEVICE_PREFIX, toPort, m_PortNrFound));
    }
    
#if __linux__
    LOG_TRACE(R"(Connected to device number "{}" at "{}{:02}.")", m_PortNrFound, DEVICE_PREFIX, m_PortNrFound - 1);
#else
    LOG_TRACE(R"(Connected to device number "{}" at "{}{:02}.")", m_PortNrFound, DEVICE_PREFIX, m_PortNrFound);
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

    // init lib
    if (TC4GetPhysicalValuesIncrement(
            &incDevVoltage, &incDevCurrent, &incDevPower, &incDevResistance,
            &incSysVoltage, &incSysCurrent, &incSysPower,
            &incSysResistance) != DLL_SUCCESS) {
        throw CommException("failed to get physical values increment.");
    }

    m_readings->readModuleID();

    // One time readings... update on every new connection
    m_readings->readAdditionalPhys();
    if (m_readings->isMaster()) {
        m_readings->readSystemPhys();
    }
    m_readings->readModulePhys();

    m_readings->getVersion()->ReadDSPVersion();

    // Default is to keep system selected !
    m_readings->selectSys();

    LOG_INFO(
        R"(Regatron device connected at port "{}", configured as "{}" with module ID "{}".)",
        m_PortNrFound, ((m_readings->isMaster()) ? "master" : "slave"),
        m_readings->getModuleID());
    m_CommStatus = CommStatus::Ok;
    return true;
}
} // namespace Regatron
