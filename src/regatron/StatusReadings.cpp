#include "StatusReadings.hpp"
#include "serialiolib.h" // NOLINT
#include <sstream>

namespace Regatron {
const std::string StatusReadings::GetMinMaxNomString() const {
    return fmt::format(R"([{},{},{},{},{},{},{},{},{},{},{},{}])",
                       m_VoltagePhysMin, m_CurrentPhysMin, m_PowerPhysMin,
                       m_ResistancePhysMin, m_VoltagePhysMax, m_CurrentPhysMax,
                       m_PowerPhysMax, m_ResistancePhysMax, m_VoltagePhysNom,
                       m_CurrentPhysNom, m_PowerPhysNom, m_ResistancePhysNom);
}

void StatusReadings::ReadErrorTree32() {
    Select();
    if (TC4ReadErrorTree32(&m_ErrorTree32Mon) != DLL_SUCCESS) {
        throw CommException(fmt::format("failed to get {} error tree",
                            Name()));
    }
    if (TC4ReadWarningTree32(&m_WarningTree32Mon) != DLL_SUCCESS) {
        throw CommException(
            fmt::format("failed to get {} module warn tree", Name()));
    }
}

void StatusReadings::Read() {
    Select();
    if (TC4GetVoltageAct(&m_ActualOutVoltageMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output voltage");
    }

    if (TC4GetPowerAct(&m_ActualOutPowerMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output power");
    }

    if (TC4GetCurrentAct(&m_ActualOutCurrentMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output current");
    }

    if (TC4GetResistanceAct(&m_ActualResMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual resistence");
    }

    if (TC4StateActSystem(&m_State) != DLL_SUCCESS) {
        throw CommException("failed to get module state");
    }
}

const std::string StatusReadings::GetReadingsString() {
    Read();
    return fmt::format(R"([{},{},{},{},{}])", m_ActualOutVoltageMon,
                       m_ActualOutCurrentMon, m_ActualOutPowerMon,
                       m_ActualResMon, m_State);
}

const std::string StatusReadings::GetErrorTreeString() {
    std::ostringstream oss;
    ReadErrorTree32();

    // Error
    oss << '[' << m_ErrorTree32Mon.group << ',';
    for (const auto &error : m_ErrorTree32Mon.error) {
        oss << error;
        oss << ',';
    }

    // Warning
    int idx{0};
    oss << m_WarningTree32Mon.group << ',';
    for (const auto &error : m_WarningTree32Mon.error) {
        oss << error;
        idx++;
        if (idx < ERROR_TREE32_LEN) {
            oss << ',';
        }
    }
    oss << ']';
    return oss.str();
}

const std::string StatusReadings::GetControlModeString() {
    ReadControlMode();
    return fmt::format("{}", m_ControlMode);
}

double StatusReadings::GetCurrentRef() {
    Select();
    if (TC4GetCurrentRef(&m_CurrentRef) != DLL_SUCCESS) {
        throw CommException("failed to read module current referece");
    }
    return m_CurrentRef;
}
double StatusReadings::GetVoltageRef() {
    Select();
    if (TC4GetVoltageRef(&m_VoltageRef) != DLL_SUCCESS) {
        throw CommException("failed to read module voltage referece");
    }
    return m_VoltageRef;
}
double StatusReadings::GetResistanceRef() {
    Select();
    if (TC4GetResistanceRef(&m_ResRef) != DLL_SUCCESS) {
        throw CommException("failed to read module resitance referece");
    }
    return m_ResRef;
}
double StatusReadings::GetPowerRef() {
    Select();
    if (TC4GetPowerRef(&m_PowerRef) != DLL_SUCCESS) {
        throw CommException("failed to read module voltage referece");
    }
    return m_PowerRef;
}

} // namespace Regatron