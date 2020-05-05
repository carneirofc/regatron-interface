#include "Comm.hpp"

#include "serialiolib.h" // NOLINT
#include <iostream>


#include "fmt/format.h"
#include "log/Logger.hpp"

namespace Regatron {
constexpr unsigned int SEARCH_SLEEP_SEC = 1000 * 1000 * 2;

Comm::Comm(int port)
    : m_port(port), m_readings(std::make_shared<Regatron::Readings>()) {
    this->m_readings->getVersion()->readDllVersion();
    LOG_INFO("initializing tcio lib");
    DllInit();
    getDllStatus();
}

Comm::Comm() : Comm(1) {}

Comm::~Comm() {
    if (DllClose() != DLL_SUCCESS) {

        LOG_ERROR("failed to close tcio lib");
    }
    LOG_INFO("regatron obj deleted");
}

void Comm::connect() { this->connect(m_port, m_port); }

void Comm::connect(int port) { this->connect(port, port); }

DllStatus Comm::getDllStatus() {
    int state;
    int errorNo;

    if (DllGetStatus(&state, &errorNo) != DLL_SUCCESS) {
        LOG_ERROR("dll status: failed to get Dll status.");
        return DllStatus::DLL_FAILURE;
    }

    switch (state) {
    case DllStatus::COMMAND_ERROR:
        LOG_ERROR(
            "dll status: device command execution error. State={}, errno={}",
            state, errorNo);
        return DllStatus::COMMAND_ERROR;

    case DllStatus::COMM_ERROR:
        LOG_ERROR("dll status: communication error. State={}, errno={}", state,
                  errorNo);
        return DllStatus::COMM_ERROR;

    case DllStatus::OK:
        LOG_INFO("dll status: OK");
        return DllStatus::OK;

    default:
        LOG_ERROR("dll status: unknown state {}", state);
        return DllStatus::DLL_FAILURE;
    }
}

void Comm::connect(int fromPort, int toPort) {
    // search device connected via DIGI RealPort adapter -> /dev/ttyD00
    if (DllSetSearchDevice2ttyDIGI() != DLL_SUCCESS) {
        throw std::runtime_error("failed to set ttyDIGI");
    }

    // search device
    usleep(SEARCH_SLEEP_SEC); // hack: while eth and rs232 at the same tc
                              // device: wait 2 sec
    LOG_INFO("searching from {} to {}", fromPort, toPort);

    if (DllSearchDevice(fromPort, toPort, &m_portNrFound) != DLL_SUCCESS) {
        throw std::runtime_error(
            fmt::format("failed to connect to device. Port range ({}, {})",
                        fromPort, toPort));
    }

    // set remote control to RS232
    if (TC4SetRemoteControlInput(2) != DLL_SUCCESS) {
        throw std::runtime_error("failed to set remote control do RS232.");
    }

    // init lib
    if (TC4GetPhysicalValuesIncrement(
            &incDevVoltage, &incDevCurrent, &incDevPower, &incDevResistance,
            &incSysVoltage, &incSysCurrent, &incSysPower,
            &incSysResistance) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get physical values increment.");
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
}

void Comm::moduleIDInfo() {
    if (this->m_portNrFound == -1) {
        LOG_WARN("not connected to any device.");
    }
    if (this->m_portNrFound != -1) {
        LOG_INFO("module connect at {} is configured as {}, module ID {}.",
                 this->m_portNrFound,
                 ((m_readings->isMaster()) ? "master" : "slave"),
                 m_readings->getModuleID());
    }
}
} // namespace Regatron
