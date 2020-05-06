#include "Readings.hpp"

namespace Regatron {

void Readings::readModuleID() {
    if (TC4GetModuleID(&(this->m_moduleID)) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module ID.");
    }
}

void Readings::readModuleErrorTree32() {
    selectMod();
    if (TC4ReadErrorTree32(&m_ModErrorTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module error tree");
    }
    if (TC4ReadWarningTree32(&m_ModWarningTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get module warn tree");
    }
}

void Readings::readModule() {

    selectMod();
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
    selectSys();
    if (TC4ReadErrorTree32(&m_SysErrorTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system error tree");
    }
    if (TC4ReadWarningTree32(&m_SysWarningTree32Mon) != DLL_SUCCESS) {
        throw std::runtime_error("failed to get system warn tree");
    }
}

void Readings::readSystem() {
    selectSys();
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

bool Readings::isMaster() const { return (m_moduleID == 0); }

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

double Readings::getModCurrentRef() {
    selectMod();
    if (TC4GetCurrentRef(&m_ModCurrentRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read module current referece");
    }
    return m_ModCurrentRef;
}
double Readings::getModVoltageRef() {
    selectMod();
    if (TC4GetVoltageRef(&m_ModVoltageRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read module voltage referece");
    }
    return m_ModVoltageRef;
}
double Readings::getModResistanceRef() {
    selectMod();
    if (TC4GetResistanceRef(&m_ModResRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read module resitance referece");
    }
    return m_ModResRef;
}
double Readings::getModPowerRef() {
    selectMod();
    if (TC4GetPowerRef(&m_ModPowerRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read module voltage referece");
    }
    return m_ModPowerRef;
}

double Readings::getSysCurrentRef() {
    selectSys();
    if (TC4GetCurrentRef(&m_SysCurrentRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read system current referece");
    }
    return m_SysCurrentRef;
}
double Readings::getSysVoltageRef() {
    selectSys();
    if (TC4GetVoltageRef(&m_SysVoltageRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read system voltage referece");
    }
    return m_SysVoltageRef;
}
double Readings::getSysResistanceRef() {
    selectSys();
    if (TC4GetResistanceRef(&m_SysResRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read system resitance referece");
    }
    return m_SysResRef;
}
double Readings::getSysPowerRef() {
    selectSys();
    if (TC4GetPowerRef(&m_SysPowerRef) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read system voltage referece");
    }
    return m_SysPowerRef;
}

void Readings::setSysCurrentRef(double value /* [A] */) {
    selectSys();
    if (TC4SetCurrentRef(value) != DLL_SUCCESS) {
        throw std::runtime_error("failed to set system current referece");
    }
}
void Readings::setSysVoltageRef(double value /* [V] */) {
    selectSys();
    if (TC4SetVoltageRef(value) != DLL_SUCCESS) {
        throw std::runtime_error("failed to set system voltage referece");
    }
}
void Readings::setSysPowerRef(double value /* [kW] */) {
    selectSys();
    if (TC4SetPowerRef(value) != DLL_SUCCESS) {
        throw std::runtime_error("failed to set system power referece");
    }
}
void Readings::setSysResistanceRef(double value /* [mOhm] */) {
    selectSys();
    if (TC4SetResistanceRef(value) != DLL_SUCCESS) {
        throw std::runtime_error("failed to set system resistance referece");
    }
}

} // namespace Regatron
