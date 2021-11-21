#include "ControllerSettings.hpp"

#include "fmt/format.h"

#include "Regatron.hpp"

namespace Regatron {

// ----------------------- Slope Voltage ---------------------------
bool ControllerSettings::SetSlopeVoltRaw(const double valRaw) {
    const auto rawVolt = static_cast<unsigned int>(valRaw);
    if (rawVolt < SLOPE_MIN_RAW || rawVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set voltage slope, "{}" is out of range "{}"<=x<="{}.)",
            rawVolt, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeVolt = rawVolt;
    return true;
}

bool ControllerSettings::SetSlopeStartupVoltRaw(const double valRaw) {
    const auto rawVolt = static_cast<unsigned int>(valRaw);
    if (rawVolt < SLOPE_MIN_RAW || rawVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup voltage slope, "{}" is out of range "{}"<=x<="{}.)",
            rawVolt, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeStartupVolt = rawVolt;
    return true;
}

bool ControllerSettings::SetSlopeVoltMs(const double valMs) {
    const unsigned int rawVoltMs = SlopeVmsToRaw(valMs);
    if (rawVoltMs < SLOPE_MIN_RAW || rawVoltMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope V/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawVoltMs);
        return false;
    }
    m_SlopeVolt = rawVoltMs;
    LOG_INFO(R"(Voltage Slope: {}V/ms = {})", valMs, m_SlopeVolt);
    return true;
}

bool ControllerSettings::SetSlopeStartupVoltMs(const double valMs) {
    const unsigned int rawVoltMs = SlopeVmsToRaw(valMs);
    if (rawVoltMs < SLOPE_MIN_RAW || rawVoltMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup slope V/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawVoltMs);
        return false;
    }
    m_SlopeStartupVolt = rawVoltMs;
    LOG_INFO(R"(Voltage Startup Slope: {}V/ms = {})", valMs, m_SlopeVolt);
    return true;
}

bool ControllerSettings::WriteSlopeVolt() {
    if (m_SlopeVolt < SLOPE_MIN_RAW || m_SlopeVolt > SLOPE_MAX_RAW ||
        m_SlopeStartupVolt < SLOPE_MIN_RAW ||
        m_SlopeStartupVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(R"(setVoltageRamp: Parameters must be >= {} and <={}.)",
                     SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    LOG_TRACE(R"(Configuring voltage slope to ({},{}) aka ({},{})V/ms)",
              m_SlopeStartupVolt, m_SlopeVolt,
              SlopeRawToVms(m_SlopeStartupVolt), SlopeRawToVms(m_SlopeVolt));

    LOG_CRITICAL("Not available");
    if (TC4SetVoltageSlopeRamp(m_SlopeVolt, m_SlopeStartupVolt) !=
        DLL_SUCCESS) {
        throw CommException("Failed to set voltage slopes");
    }
    LOG_TRACE(R"(Voltage slope results are: "{}" "{}".)", m_SlopeStartupVolt,
              m_SlopeVolt);
    return true;
}

/**
 * @return: String formatted as an array containing:
 * [rawStartup,raw,V/ms statup,V/ms]
 * */
std::string ControllerSettings::GetSlopeVolt() {
    unsigned int startupValue{};
    unsigned int value{};
    if (TC4GetVoltageSlopeRamp(&value, &startupValue) != DLL_SUCCESS) {
        throw CommException("failed to get voltage slope ramp values.");
    }
    return fmt::format("[{},{},{},{}]", startupValue, value,
                       SlopeRawToVms(startupValue), SlopeRawToVms(value));
}

// ----------------------- Slope Current ---------------------------
bool ControllerSettings::SetSlopeCurrentRaw(const double valRaw) {
    const auto rawCurrent = static_cast<unsigned int>(valRaw);
    if (rawCurrent < SLOPE_MIN_RAW || rawCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set current slope, "{}" is out of range "{}"<=x<="{}.)",
            rawCurrent, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeCurrent = rawCurrent;
    return true;
}

bool ControllerSettings::SetSlopeStartupCurrentRaw(const double valRaw) {
    const auto rawCurrent = static_cast<unsigned int>(valRaw);
    if (rawCurrent < SLOPE_MIN_RAW || rawCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set current startup slope, "{}" is out of range "{}"<=x<="{}.)",
            rawCurrent, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeStartupCurrent = rawCurrent;
    return true;
}

bool ControllerSettings::SetSlopeCurrentMs(const double valMs) {
    const unsigned int rawCurrentMs = SlopeAmsToRaw(valMs);
    if (rawCurrentMs < SLOPE_MIN_RAW || rawCurrentMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope A/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawCurrentMs);
        return false;
    }
    m_SlopeCurrent = rawCurrentMs;
    LOG_INFO(R"(Current Slope: {}A/ms = {})", valMs, m_SlopeCurrent);
    return true;
}

bool ControllerSettings::SetSlopeStartupCurrentMs(const double valMs) {
    const unsigned int rawCurrentMs = SlopeAmsToRaw(valMs);
    if (rawCurrentMs < SLOPE_MIN_RAW || rawCurrentMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup slope A/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawCurrentMs);
        return false;
    }
    m_SlopeStartupCurrent = rawCurrentMs;
    LOG_INFO(R"(Current Startup Slope: {}A/ms = {})", valMs,
             m_SlopeStartupCurrent);
    return true;
}

bool ControllerSettings::WriteSlopeCurrent() {
    if (m_SlopeCurrent < SLOPE_MIN_RAW || m_SlopeCurrent > SLOPE_MAX_RAW ||
        m_SlopeStartupCurrent < SLOPE_MIN_RAW ||
        m_SlopeStartupCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(setCurrentRamp: Parameters "{}" and "{}" must be >= {} and <={}.)",
            m_SlopeStartupCurrent, m_SlopeCurrent, SLOPE_MIN_RAW,
            SLOPE_MAX_RAW);
        return false;
    }
    LOG_TRACE(R"(Configuring current slope to ({},{}) aka ({},{})A/ms)",
              m_SlopeStartupCurrent, m_SlopeCurrent,
              SlopeRawToAms(m_SlopeStartupCurrent),
              SlopeRawToAms(m_SlopeCurrent));

    LOG_CRITICAL("Not available");
    if (TC4SetCurrentSlopeRamp(m_SlopeCurrent, m_SlopeStartupCurrent) !=
        DLL_SUCCESS) {
        throw CommException("Failed to set current slopes");
    }
    LOG_TRACE(R"(Currentage slope results are: "{}" "{}".)",
              m_SlopeStartupCurrent, m_SlopeCurrent);
    return true;
}

/**
 * @return: String formatted as an array containing:
 * [rawStartup,raw,A/ms statup,A/ms]
 * */
std::string ControllerSettings::GetSlopeCurrent() {
    unsigned int startupValue{};
    unsigned int value{};
    if (TC4GetCurrentSlopeRamp(&value, &startupValue) != DLL_SUCCESS) {
        throw CommException("failed to get current slope ramp values.");
    }
    return fmt::format("[{},{},{},{}]", startupValue, value,
                       SlopeRawToAms(startupValue), SlopeRawToAms(value));
}

}
