#pragma once

#include "SystemStatusReadings.hpp"

#include <memory>

namespace Regatron {

 // Slope
 static constexpr double SLOPE_MIN_TIME_MS = 0.05;
 static constexpr double SLOPE_MAX_TIME_MS = 1600.;
 static constexpr double SLOPE_MIN_RAW     = 1.;
 static constexpr double SLOPE_MAX_RAW     = 32000.;

 class ControllerSettings {
  public:
    explicit ControllerSettings(SystemStatusReadings &sys)
        : m_SysStatusReadings(sys){};
    // -------------- Slopes -------------------
    bool SetSlopeStartupVoltMs(double valMs);
    bool SetSlopeVoltMs(double valMs);
    bool SetSlopeStartupVoltRaw(double valRaw);
    bool SetSlopeVoltRaw(double valRaw);

    bool SetSlopeStartupCurrentMs(double valMs);
    bool SetSlopeCurrentMs(double valMs);
    bool SetSlopeStartupCurrentRaw(double valRaw);
    bool SetSlopeCurrentRaw(double valRaw);

    [[nodiscard]] inline double GetSlopeVoltSp() const {
        return SlopeRawToVms(m_SlopeVolt);
    }
    [[nodiscard]] inline double GetSlopeStartupVoltSp() const {
        return SlopeRawToVms(m_SlopeStartupVolt);
    }
    [[nodiscard]] inline double GetSlopeVoltMin() const {
        return SlopeRawToVms(static_cast<unsigned int>(SLOPE_MIN_RAW));
    }
    [[nodiscard]] inline double GetSlopeVoltMax() const {
        return SlopeRawToVms(static_cast<unsigned int>(SLOPE_MAX_RAW));
    }

    [[nodiscard]] inline double GetSlopeCurrentSp() const {
        return SlopeRawToAms(m_SlopeCurrent);
    }
    [[nodiscard]] inline double GetSlopeStartupCurrentSp() const {
        return SlopeRawToAms(m_SlopeStartupCurrent);
    }
    [[nodiscard]] inline double GetSlopeCurrentMin() const {
        return SlopeRawToAms(static_cast<unsigned int>(SLOPE_MIN_RAW));
    }
    [[nodiscard]] inline double GetSlopeCurrentMax() const {
        return SlopeRawToAms(static_cast<unsigned int>(SLOPE_MAX_RAW));
    }

    bool WriteSlopeVolt();
    bool WriteSlopeCurrent();
    std::string GetSlopeVolt();
    std::string GetSlopeCurrent();

    /**
     * 1:     slowest set value ramp: (Min V/ms) 0-100% (full scale) in 1.6s
     * 32000: fastest set value ramp: (Max V/ms) 0-100% (full scale) in 50us
     *
     * SLOPE_MIN_RAW = a*MIN_VOLT_MS + b
     * SLOPE_MAX_RAW = a*MAX_VOLT_MS + b
     *
     * a=(SLOPE_MIN_RAW-SLOPE_MAX_RAW)/(MIN_VOLT_MS-MAX_VOLT_MS)
     * b=(SLOPE_MAX_RAW*MIN_VOLT_MS-MAX_VOLT_MS*SLOPE_MIN_RAW)/(MIN_VOLT_MS-MAX_VOLT_MS)
     */
    [[nodiscard]] static double GetSlopeA(const double fullScaleValue) {
        const double MIN_VALUE_MS = fullScaleValue / SLOPE_MAX_TIME_MS;
        const double MAX_VALUE_MS = fullScaleValue / SLOPE_MIN_TIME_MS;
        return (SLOPE_MIN_RAW - SLOPE_MAX_RAW) / (MIN_VALUE_MS - MAX_VALUE_MS);
    }

    [[nodiscard]] static double GetSlopeB(const double fullScaleValue) {
        const double MIN_VALUE_MS = fullScaleValue / SLOPE_MAX_TIME_MS;
        const double MAX_VALUE_MS = fullScaleValue / SLOPE_MIN_TIME_MS;
        return (SLOPE_MAX_RAW * MIN_VALUE_MS - MAX_VALUE_MS * SLOPE_MIN_RAW) /
               (MIN_VALUE_MS - MAX_VALUE_MS);
    }

    [[nodiscard]] uint32_t SlopeVmsToRaw(const double voltms) const {
        return static_cast<uint32_t>(
            std::round(GetSlopeA(m_SysStatusReadings.GetVoltagePhysMax()) * voltms +
                       GetSlopeB(m_SysStatusReadings.GetVoltagePhysMax())));
    }

    [[nodiscard]] unsigned int SlopeAmsToRaw(const double currentms) const {
        return static_cast<unsigned int>(
            std::round(GetSlopeA(m_SysStatusReadings.GetCurrentPhysMax()) * currentms +
                       GetSlopeB(m_SysStatusReadings.GetCurrentPhysMax())));
    }

    [[nodiscard]] double SlopeRawToVms(const unsigned int raw) const {
        return (static_cast<double>(raw) - GetSlopeB(m_SysStatusReadings.GetVoltagePhysMax()) /
               GetSlopeA(m_SysStatusReadings.GetVoltagePhysMax()));
    }

    [[nodiscard]] double SlopeRawToAms(const unsigned int raw) const {
        return (static_cast<double>(raw) - GetSlopeB(m_SysStatusReadings.GetCurrentPhysMax()) /
               GetSlopeA(m_SysStatusReadings.GetCurrentPhysMax()));
    }

  private: 
    SystemStatusReadings &m_SysStatusReadings;
    // Master only ...
    unsigned int m_SlopeStartupVolt    = 0;
    unsigned int m_SlopeStartupCurrent = 0;
    unsigned int m_SlopeVolt           = 0;
    unsigned int m_SlopeCurrent        = 0;
 };
}
