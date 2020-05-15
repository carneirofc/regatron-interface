
#include "Comm.hpp"

namespace Regatron {
constexpr auto         DELAY_RS232              = std::chrono::seconds{2};
constexpr unsigned int READ_TIMEOUT_MULTIPLIER  = 40;
constexpr unsigned int WRITE_TIMEOUT_MULTIPLIER = 40;
constexpr const char* DEVICE_PREFIX = "/dev/ttyD";
constexpr int          DLL_STATUS_OK                  = 0;
constexpr int          DLL_STATUS_COMMUNICATION_ERROR = -10;
constexpr int          DLL_STATUS_COMMAND_EXECUTION_ERROR = -100;

Comm::Comm(int port)
    : m_Port(port), m_readings(std::make_shared<Regatron::Readings>()),
      m_CommStatus{CommStatus::Disconncted}, m_AutoReconnect(false),
      m_Connected(false) {
    m_readings->getVersion()->readDllVersion();
}

Comm::Comm() : Comm(1) {}

Comm::~Comm() {
    disconnect();
    LOG_TRACE("Comm destroyed!");
}

void Comm::disconnect() {
    auto result = DllClose();
    m_Connected = false;
    LOG_WARN(
        R"(Dllclose: Driver/Objects used by the TCIO are closed, released memory (code "{}"))",
        result);
}
void Comm::CheckDLLStatus() {
    int state{-1};
    int errorNo{0};

    LOG_TRACE("Reading DLL status.");
    if (DllGetStatus(&state, &errorNo) != DLL_SUCCESS) {
        throw CommException("dll status: failed to get Dll status.",
                            CommStatus::DLLFail);
    }
    switch (state) {
    case DLL_STATUS_OK:
        m_CommStatus = CommStatus::Ok;
        break;
    case DLL_STATUS_COMMUNICATION_ERROR:
        m_CommStatus = CommStatus::DLLFail;
        break;
    case DLL_STATUS_COMMAND_EXECUTION_ERROR:
        m_CommStatus = CommStatus::DLLCommFail;
    }
    if (state != DLL_STATUS_OK) {
        throw CommException("dll status: Invalid return status !.",
                            CommStatus::DLLFail);
    }
}
void Comm::InitializeDLL() {
    LOG_TRACE("Initializing TCIO lib.");
    if (DllInit() != DLL_SUCCESS) {
        throw CommException("Failed to initialize TCIO lib.",
                            CommStatus::DLLFail);
    }
    CheckDLLStatus();
}
void Comm::connect() { connect(m_Port, m_Port); }
void Comm::connect(int port) { connect(port, port); }
void Comm::connect(int fromPort, int toPort) {
    if (m_Connected) {
        throw CommException(
            R"(Already connected to a device, consider using "cmdDisconnect")");
    }

    if (fromPort < 0 || toPort < 0 || fromPort > toPort) {
        throw CommException(
            fmt::format(R"(invalid port range [{},{}].)", fromPort, toPort));
    }

    InitializeDLL();

    if (DllSetSearchDevice2ttyDIGI() != DLL_SUCCESS) {
        throw CommException("failed to set ttyDIGI string pattern.");
     }

   if (fromPort == toPort) {
       LOG_INFO(R"(searching DIGI RealPort device "{}{:02}")",
               DEVICE_PREFIX,
               fromPort);
   } else {
       LOG_INFO(
           R"(searching DIGI RealPort device in range "{}{:02}" to "{}{:02}")",
           DEVICE_PREFIX,fromPort,DEVICE_PREFIX,toPort);
   }

   // use this function for VM or  rs232 over ethernet
   // DllSetCommTimeouts(READ_TIMEOUT_MULTIPLIER, WRITE_TIMEOUT_MULTIPLIER);

   // hack: while eth and rs232 at the same tc device: wait 2 sec
   std::this_thread::sleep_for(DELAY_RS232);

   if (DllSearchDevice(fromPort + 1, toPort + 1, &m_PortNrFound) !=
           DLL_SUCCESS || m_PortNrFound == -1) {
       throw CommException(fmt::format(
           R"(failed to connect to a device in range "{}{:02}" to "{}{:02}", PortNrFound "{}".)",
           DEVICE_PREFIX, fromPort, DEVICE_PREFIX, toPort, m_PortNrFound));
    }
    LOG_TRACE(R"(Connected to device number "{}" at "{}{:02}")", m_PortNrFound,
              DEVICE_PREFIX, m_PortNrFound + 1);
    m_Connected = true;

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

    // Default is to keep system selected !
    m_readings->selectSys();

    LOG_INFO(
        R"(Regatron device connected at port "{}", configured as "{}" with module ID "{}".)",
        m_PortNrFound, ((m_readings->isMaster()) ? "master" : "slave"),
        m_readings->getModuleID());
    m_CommStatus = CommStatus::Ok;
}
} // namespace Regatron
