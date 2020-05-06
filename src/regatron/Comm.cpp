
#include "Comm.hpp"

namespace Regatron {
constexpr unsigned int SEARCH_SLEEP_SEC = 1000 * 1000 * 2;

Comm::Comm(int port)
    : m_Port(port), m_readings(std::make_shared<Regatron::Readings>()),
      m_AutoReconnect(false), m_CommStatus{CommStatus::Disconncted} {
    m_readings->getVersion()->readDllVersion();
    LOG_INFO("initializing tcio lib");
    InitializeDLL();
}

Comm::Comm() : Comm(1) {}

Comm::~Comm() {
    if (DllClose() != DLL_SUCCESS) {
        LOG_ERROR("failed to close tcio lib");
    }
    LOG_INFO("regatron obj deleted");
}

void Comm::InitializeDLL() {
    DllInit();

    int state{-1};
    int errorNo{0};

    if (DllGetStatus(&state, &errorNo) != DLL_SUCCESS) {
        throw CommException("dll status: failed to get Dll status.",
                            CommStatus::DLLFail);
    }
}

void Comm::connect() { connect(m_Port, m_Port); }
void Comm::connect(int port) { connect(port, port); }
void Comm::connect(int fromPort, int toPort) {

    // search device connected via DIGI RealPort adapter -> /dev/ttyD00
    // set search string example: "/dev/ttyS0", "/dev/ttyUSB0", "/dev/ttyD00"
    if (DllSetSearchDevice2ttyDIGI() != DLL_SUCCESS) {
        throw CommException("failed to set digi string pattern");
    }

    if (fromPort == toPort) {
        LOG_INFO(R"(searching DIGI RealPort device "/dev/ttyD{:02}")",
                 fromPort);
    } else {
        LOG_INFO(
            R"(searching DIGI RealPort device in range "/dev/ttyD{:02}" to "/dev/ttyD{:02}")",
            fromPort, toPort);
    }

    // search device
    usleep(SEARCH_SLEEP_SEC); // hack: while eth and rs232 at the same tc
                              // device: wait 2 sec

    if (DllSearchDevice(fromPort, toPort, &m_PortNrFound) != DLL_SUCCESS ||
        m_PortNrFound == -1) {
        throw CommException(fmt::format(
            R"(failed to connect to a device in range Port range "/dev/ttyD{:02}" to "/dev/ttyD{:02})",
            fromPort, toPort));
    }

    // set remote control to RS232
    if (TC4SetRemoteControlInput(2) != DLL_SUCCESS) {
        throw CommException("failed to set remote control do RS232.");
    }

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
        R"(regatron device connected at port "{}", configured as "{}" with module ID "{}".)",
        m_PortNrFound, ((m_readings->isMaster()) ? "master" : "slave"),
        m_readings->getModuleID());
    m_CommStatus = CommStatus::Ok;
}

void Comm::disconnect() { LOG_WARN("disconnecting ..."); }

} // namespace Regatron
