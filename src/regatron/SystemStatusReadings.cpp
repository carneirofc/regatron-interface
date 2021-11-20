#include "SystemStatusReadings.hpp"
#include "DeviceAccessControl.hpp"

#include "serialiolib.h" // NOLINT

namespace Regatron {

const char* SystemStatusReadings::Name() const {
    return "System Status Readings";
}

void SystemStatusReadings::Select(){
    DeviceAccessControl::SelectSys();
}

void SystemStatusReadings::ReadControlMode() {
    DeviceAccessControl::SelectSys();
    if (TC4GetControlMode(&m_ControlMode) != DLL_SUCCESS) {
        throw CommException(
            fmt::format("failed to read {} control mode", Name()));
    }
}

void SystemStatusReadings::ReadPhys() {
    if (TC4GetSystemPhysicalLimitMax(&m_VoltagePhysMax, &m_CurrentPhysMax,
                                     &m_PowerPhysMax,
                                     &m_ResistancePhysMax) != DLL_SUCCESS) {
        throw CommException(fmt::format(
            "failed to get system physical max limit values.", Name()));
    }

    if (TC4GetSystemPhysicalLimitMin(&m_VoltagePhysMin, &m_CurrentPhysMin,
                                     &m_PowerPhysMin,
                                     &m_ResistancePhysMin) != DLL_SUCCESS) {
        throw CommException(fmt::format(
            "failed to get system physical min limit values.", Name()));
    }

    if (TC4GetSystemPhysicalLimitNom(&m_VoltagePhysNom, &m_CurrentPhysNom,
                                     &m_PowerPhysNom,
                                     &m_ResistancePhysNom) != DLL_SUCCESS) {
        throw CommException(fmt::format(
            "failed to get system physical nominal values.", Name()));
    }
}
} // namespace Regatron
