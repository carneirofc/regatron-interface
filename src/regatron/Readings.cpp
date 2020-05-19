#include "Readings.hpp"

namespace Regatron {

/**
 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 32000: fastest set value ramp: 0-100% (full scale) in 50us
 */
unsigned int Slope::timeToRaw(double y /* [5e-5 to 1.6] s */) {
    return static_cast<unsigned int>((y - Slope::SLOPE_B) / Slope::SLOPE_A);
}

/**
 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 32000: fastest set value ramp: 0-100% (full scale) in 50us
 */
double Slope::rawToTime(double x) {
    return Slope::SLOPE_A * x + Slope::SLOPE_B;
}

void Readings::readModuleID() {
    if (TC4GetModuleID(&(this->m_moduleID)) != DLL_SUCCESS) {
        throw CommException("failed to get module ID.");
    }
}

void Readings::readModuleErrorTree32() {
    selectMod();
    if (TC4ReadErrorTree32(&m_ModErrorTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get module error tree");
    }
    if (TC4ReadWarningTree32(&m_ModWarningTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get module warn tree");
    }
}

void Readings::readModule() {

    selectMod();
    if (TC4GetVoltageActSense(&m_ModActualOutVoltageMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output voltage");
    }

    if (TC4GetPowerActSense(&m_ModActualOutPowerMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output power");
    }

    if (TC4GetCurrentAct(&m_ModActualOutCurrentMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output current");
    }

    if (TC4GetResistanceAct(&m_ModActualResMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual resistence");
    }

    if (TC4StateActSystem(&m_ModState) != DLL_SUCCESS) {
        throw CommException("failed to get module state");
    }
}

void Readings::readSystemErrorTree32() {
    selectSys();
    if (TC4ReadErrorTree32(&m_SysErrorTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get system error tree");
    }
    if (TC4ReadWarningTree32(&m_SysWarningTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get system warn tree");
    }
}

void Readings::readSystem() {
    selectSys();
    if (TC4GetVoltageActSense(&m_SysActualOutVoltageMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual output voltage");
    }

    if (TC4GetPowerActSense(&m_SysActualOutPowerMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual output power");
    }

    if (TC4GetCurrentAct(&m_SysActualOutCurrentMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual output current");
    }

    if (TC4GetResistanceAct(&m_SysActualResMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual resistence");
    }

    if (TC4StateActSystem(&m_SysState) != DLL_SUCCESS) {
        throw CommException("failed to get system state");
    }
}

bool Readings::isMaster() const { return (m_moduleID == 0); }

void Readings::readAdditionalPhys() {
    if (TC4GetAdditionalPhysicalValues(&m_DCLinkPhysNom,
                                       &m_PrimaryCurrentPhysNom,
                                       &m_TemperaturePhysNom) != DLL_SUCCESS) {
        throw CommException("failed to get additional physical values.");
    }
}

void Readings::readModulePhys() {
    if (TC4GetModulePhysicalLimitMax(&m_ModVoltagePhysMax, &m_ModCurrentPhysMax,
                                     &m_ModPowerPhysMax,
                                     &m_ModResistancePhysMax) != DLL_SUCCESS) {
        throw CommException(
            "failed to get module physical max limit values.");
    }

    if (TC4GetModulePhysicalLimitMin(&m_ModVoltagePhysMin, &m_ModCurrentPhysMin,
                                     &m_ModPowerPhysMin,
                                     &m_ModResistancePhysMin) != DLL_SUCCESS) {
        throw CommException(
            "failed to get module physical min limit values.");
    }

    if (TC4GetModulePhysicalLimitNom(&m_ModVoltagePhysNom, &m_ModCurrentPhysNom,
                                     &m_ModPowerPhysNom,
                                     &m_ModResistancePhysNom) != DLL_SUCCESS) {
        throw CommException(
            "failed to get module physical nominal values.");
    }
}

void Readings::readSystemPhys() {

    if (TC4GetSystemPhysicalLimitMax(&m_SysVoltagePhysMax, &m_SysCurrentPhysMax,
                                     &m_SysPowerPhysMax,
                                     &m_SysResistancePhysMax) != DLL_SUCCESS) {
        throw CommException(
            "failed to get system physical max limit values.");
    }

    if (TC4GetSystemPhysicalLimitMin(&m_SysVoltagePhysMin, &m_SysCurrentPhysMin,
                                     &m_SysPowerPhysMin,
                                     &m_SysResistancePhysMin) != DLL_SUCCESS) {
        throw CommException(
            "failed to get system physical min limit values.");
    }

    if (TC4GetSystemPhysicalLimitNom(&m_SysVoltagePhysNom, &m_SysCurrentPhysNom,
                                     &m_SysPowerPhysNom,
                                     &m_SysResistancePhysNom) != DLL_SUCCESS) {
        throw CommException(
            "failed to get system physical nominal values.");
    }
}

std::string Readings::getSysMinMaxNom() {
    return fmt::format(
        R"([{},{},{},{},{},{},{},{},{},{},{},{}])", m_SysVoltagePhysMin,
        m_SysCurrentPhysMin, m_SysPowerPhysMin, m_SysResistancePhysMin,
        m_SysVoltagePhysMax, m_SysCurrentPhysMax, m_SysPowerPhysMax,
        m_SysResistancePhysMax, m_SysVoltagePhysNom, m_SysCurrentPhysNom,
        m_SysPowerPhysNom, m_SysResistancePhysNom);
}

std::string Readings::getModMinMaxNom() {
    return fmt::format(
        R"([{},{},{},{},{},{},{},{},{},{},{},{}])", m_ModVoltagePhysMin,
        m_ModCurrentPhysMin, m_ModPowerPhysMin, m_ModResistancePhysMin,
        m_ModVoltagePhysMax, m_ModCurrentPhysMax, m_ModPowerPhysMax,
        m_ModResistancePhysMax, m_ModVoltagePhysNom, m_ModCurrentPhysNom,
        m_ModPowerPhysNom, m_ModResistancePhysNom);
}

double Readings::getModCurrentRef() {
    selectMod();
    if (TC4GetCurrentRef(&m_ModCurrentRef) != DLL_SUCCESS) {
        throw CommException("failed to read module current referece");
    }
    return m_ModCurrentRef;
}
double Readings::getModVoltageRef() {
    selectMod();
    if (TC4GetVoltageRef(&m_ModVoltageRef) != DLL_SUCCESS) {
        throw CommException("failed to read module voltage referece");
    }
    return m_ModVoltageRef;
}
double Readings::getModResistanceRef() {
    selectMod();
    if (TC4GetResistanceRef(&m_ModResRef) != DLL_SUCCESS) {
        throw CommException("failed to read module resitance referece");
    }
    return m_ModResRef;
}
double Readings::getModPowerRef() {
    selectMod();
    if (TC4GetPowerRef(&m_ModPowerRef) != DLL_SUCCESS) {
        throw CommException("failed to read module voltage referece");
    }
    return m_ModPowerRef;
}

double Readings::getSysCurrentRef() {
    selectSys();
    if (TC4GetCurrentRef(&m_SysCurrentRef) != DLL_SUCCESS) {
        throw CommException("failed to read system current referece");
    }
    return m_SysCurrentRef;
}
double Readings::getSysVoltageRef() {
    selectSys();
    if (TC4GetVoltageRef(&m_SysVoltageRef) != DLL_SUCCESS) {
        throw CommException("failed to read system voltage referece");
    }
    return m_SysVoltageRef;
}
double Readings::getSysResistanceRef() {
    selectSys();
    if (TC4GetResistanceRef(&m_SysResRef) != DLL_SUCCESS) {
        throw CommException("failed to read system resitance referece");
    }
    return m_SysResRef;
}
double Readings::getSysPowerRef() {
    selectSys();
    if (TC4GetPowerRef(&m_SysPowerRef) != DLL_SUCCESS) {
        throw CommException("failed to read system voltage referece");
    }
    return m_SysPowerRef;
}

void Readings::setSysCurrentRef(double value /* [A] */) {
    selectSys();
    if (TC4SetCurrentRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system current referece");
    }
}
void Readings::setSysVoltageRef(double value /* [V] */) {
    selectSys();
    if (TC4SetVoltageRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system voltage referece");
    }
}
void Readings::setSysPowerRef(double value /* [kW] */) {
    selectSys();
    if (TC4SetPowerRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system power referece");
    }
}
void Readings::setSysResistanceRef(double value /* [mOhm] */) {
    selectSys();
    if (TC4SetResistanceRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system resistance referece");
    }
}

std::string Readings::getModTree() {
    std::ostringstream oss;
    readModuleErrorTree32();

    // Error
    oss << '[' << m_ModErrorTree32Mon.group << ',';
    for (const auto &error : m_ModErrorTree32Mon.error) {
        oss << error;
        oss << ',';
    }

    // Warning
    int idx{0};
    oss << m_ModWarningTree32Mon.group << ',';
    for (const auto &error : m_ModWarningTree32Mon.error) {
        oss << error;
        idx++;
        if (idx < errorTree32Len) {
            oss << ',';
        }
    }
    oss << ']';
    return oss.str();
}

std::string Readings::getSysTree() {
    std::ostringstream oss;
    readSystemErrorTree32();

    // Error
    oss << '[' << m_SysErrorTree32Mon.group << ',';
    for (const auto &error : m_SysErrorTree32Mon.error) {
        oss << error;
        oss << ',';
    }

    // Warning
    int idx{0};
    oss << m_SysWarningTree32Mon.group << ',';
    for (const auto &error : m_SysWarningTree32Mon.error) {
        oss << error;
        idx++;
        if (idx < errorTree32Len) {
            oss << ',';
        }
    }
    oss << ']';
    return oss.str();
}

void Readings::setSysOutVoltEnable(unsigned int state) {
    if (TC4SetControlIn(state) != DLL_SUCCESS) {
        throw CommException("failed to set system output voltage state");
    }
}

bool Readings::getSysOutVoltEnable() {
    if (TC4GetControlIn(&m_SysOutVoltEnable) != DLL_SUCCESS) {
        throw CommException("failed to get system output voltage state");
    }
    return static_cast<bool>(m_SysOutVoltEnable);
}
} // namespace Regatron
