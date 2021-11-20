#include "ModuleStatusReadings.hpp"
#include "serialiolib.h" // NOLINT
#include "DeviceAccessControl.hpp"

namespace Regatron {

const char* ModuleStatusReadings::Name() const {
    return "Module Status Readings";
};

void ModuleStatusReadings::Select() {
    DeviceAccessControl::SelectMod();
};


void ModuleStatusReadings::ReadControlMode() {
    DeviceAccessControl::SelectMod();
    if (TC4GetControlMode(&m_ControlMode) != DLL_SUCCESS) {
        throw CommException("failed to read module control mode");
    }
    DeviceAccessControl::SelectSys();
}

void ModuleStatusReadings::ReadPhys() {
    if (TC4GetModulePhysicalLimitMax(&m_VoltagePhysMax, &m_CurrentPhysMax,
                                     &m_PowerPhysMax,
                                     &m_ResistancePhysMax) != DLL_SUCCESS) {
        throw CommException(fmt::format(
            "failed to get {} physical max limit values.", Name()));
    }

    if (TC4GetModulePhysicalLimitMin(&m_VoltagePhysMin, &m_CurrentPhysMin,
                                     &m_PowerPhysMin,
                                     &m_ResistancePhysMin) != DLL_SUCCESS) {
        throw CommException(fmt::format(
            "failed to get {} physical min limit values.", Name()));
    }

    if (TC4GetModulePhysicalLimitNom(&m_VoltagePhysNom, &m_CurrentPhysNom,
                                     &m_PowerPhysNom,
                                     &m_ResistancePhysNom) != DLL_SUCCESS) {
        throw CommException(fmt::format(
            "failed to get {} physical nominal values.", Name()));
    }
}

} // namespace Regatron