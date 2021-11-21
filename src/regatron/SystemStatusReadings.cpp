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

void SystemStatusReadings::SetCurrentRef(double value /* [A] */) {
    Select();
    if (TC4SetCurrentRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system current referece");
    }
}

void SystemStatusReadings::SetVoltageRef(double value /* [V] */) {
    Select();
    if (TC4SetVoltageRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system voltage referece");
    }
}

void SystemStatusReadings::SetPowerRef(double value /* [kW] */) {
    Select();
    if (TC4SetPowerRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system power referece");
    }
}

void SystemStatusReadings::SetResistanceRef(double value /* [mOhm] */) {
    Select();
    if (TC4SetResistanceRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system resistance referece");
    }
}

void SystemStatusReadings::SetOutVoltEnable(uint32_t state) {
    if (TC4SetControlIn(state) != DLL_SUCCESS) {
        throw CommException("failed to set system output voltage state");
    }
}

int SystemStatusReadings::GetOutVoltEnable() {
    if (TC4GetControlIn(&m_OutVoltEnable) != DLL_SUCCESS) {
        throw CommException("failed to get system output voltage state");
    }
    return static_cast<int>(m_OutVoltEnable);
}

} // namespace Regatron
