#include "Readings.hpp"

namespace Regatron {

void Readings::readModuleID() {
    if (TC4GetModuleID(&(this->m_moduleID)) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module ID.");
    }
}

void Readings::readModuleErrorTree32() {
    selectModule();
    if (TC4ReadErrorTree32(&m_ModErrorTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module error tree");
    }
    if (TC4ReadWarningTree32(&m_ModWarningTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module warn tree");
    }
}

void Readings::readModule() {

    selectModule();
    if (TC4GetVoltageActSense(&m_ModActualOutVoltageMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module actual output voltage");
    }

    if (TC4GetPowerActSense(&m_ModActualOutPowerMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module actual output power");
    }

    if (TC4GetCurrentAct(&m_ModActualOutCurrentMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module actual output current");
    }

    if (TC4GetResistanceAct(&m_ModActualResMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module actual resistence");
    }

    if (TC4StateActSystem(&m_ModState) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module state");
    }
}

void Readings::readSystemErrorTree32() {
    selectSystem();
    if (TC4ReadErrorTree32(&m_SysErrorTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system error tree");
    }
    if (TC4ReadWarningTree32(&m_SysWarningTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system warn tree");
    }
}

void Readings::readSystem() {
    selectSystem();
    if (TC4GetVoltageActSense(&m_SysActualOutVoltageMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system actual output voltage");
    }

    if (TC4GetPowerActSense(&m_SysActualOutPowerMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system actual output power");
    }

    if (TC4GetCurrentAct(&m_SysActualOutCurrentMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system actual output current");
    }

    if (TC4GetResistanceAct(&m_SysActualResMon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system actual resistence");
    }

    if (TC4StateActSystem(&m_SysState) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system state");
    }
}

bool Readings::isMaster() { return (m_moduleID == 0); }

void Readings::readAdditionalPhys() {
    if (TC4GetAdditionalPhysicalValues(&m_DCLinkPhysNom,
                                       &m_PrimaryCurrentPhysNom,
                                       &m_TemperaturePhysNom) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get additional physical values.");
    }
}

void Readings::readModulePhys() {
    if (TC4GetModulePhysicalLimitMax(&m_ModVoltagePhysMax, &m_ModCurrentPhysMax,
                                     &m_ModPowerPhysMax,
                                     &m_ModResistancePhysMax) != DLL_SUCCESS) {
        throw std::runtime_error(
            "failed to get module physical max limit values.");
    }

    if (TC4GetModulePhysicalLimitMin(&m_ModVoltagePhysMin, &m_ModCurrentPhysMin,
                                     &m_ModPowerPhysMin,
                                     &m_ModResistancePhysMin) != DLL_SUCCESS) {
        throw std::runtime_error(
            "failed to get module physical min limit values.");
    }

    if (TC4GetModulePhysicalLimitNom(&m_ModVoltagePhysNom, &m_ModCurrentPhysNom,
                                     &m_ModPowerPhysNom,
                                     &m_ModResistancePhysNom) != DLL_SUCCESS) {
        throw std::runtime_error(
            "failed to get module physical nominal values.");
    }
}

void Readings::readSystemPhys() {

    if (TC4GetSystemPhysicalLimitMax(&m_SysVoltagePhysMax, &m_SysCurrentPhysMax,
                                     &m_SysPowerPhysMax,
                                     &m_SysResistancePhysMax) != DLL_SUCCESS) {
        throw std::runtime_error(
            "failed to get system physical max limit values.");
    }

    if (TC4GetSystemPhysicalLimitMin(&m_SysVoltagePhysMin, &m_SysCurrentPhysMin,
                                     &m_SysPowerPhysMin,
                                     &m_SysResistancePhysMin) != DLL_SUCCESS) {
        throw std::runtime_error(
            "failed to get system physical min limit values.");
    }

    if (TC4GetSystemPhysicalLimitNom(&m_SysVoltagePhysNom, &m_SysCurrentPhysNom,
                                     &m_SysPowerPhysNom,
                                     &m_SysResistancePhysNom) != DLL_SUCCESS) {
        throw std::runtime_error(
            "failed to get system physical nominal values.");
    }
}
} // namespace Regatron
