#pragma once

#include <cmath>
#include <memory>
#include <sstream>
#include <stdint.h>

#include "StatusReadings.hpp"
#include "ModuleStatusReadings.hpp"
#include "SystemStatusReadings.hpp"
#include "DeviceAccessControl.hpp"

#include "Version.hpp"
#include "log/Logger.hpp"

namespace Regatron {


namespace ControlMode {
/**
0: no controller selected (output voltage is disabled)
1: constant voltage
2: constant current
4: constant power
*/
constexpr unsigned int OUTPUT_VOLTAGE_OFF = 0;
constexpr unsigned int CONST_VOLTAGE      = 0;
constexpr unsigned int CONST_CURRENT      = 0;
constexpr unsigned int CONST_POWER        = 0;
} // namespace ControlMode

/* Remote Control Input
   0: set active interface to analog
   1: set active interface to HMI/HME
   2: set active interface to RS232 (this API)
   32767: no active interface will be selected
*/



class Readings {
  public:
    Readings()
        : m_Version({}),
            m_SysStatusReadings(SystemStatusReadings()),
            m_ModStatusReadings(ModuleStatusReadings()),
            m_IBCInvHeatsinkTemp(0),
            m_DCLinkPhysNom(0),
            m_PrimaryCurrentPhysNom(0),
            m_TemperaturePhysNom(0),

            m_SysOutVoltEnable(0),

            m_RemoteCtrlInp(0),
            m_DCLinkVoltageMon(0),
            m_PrimaryCurrentMon(0),
            m_IGBTTempMon(0),
            m_RectifierTempMon(0),
            m_PCBTempMon(0),

            m_SlopeStartupVolt(0),
            m_SlopeStartupCurrent(0),
            m_SlopeVolt(0),
            m_SlopeCurrent(0),

            m_ModuleID(0),
            m_OperatingSeconds(0),
            m_PowerupTimeSeconds(0),
            m_FlashErrorHistoryMaxEntries(30)
    {}
  private:
    /** One time readings */

    // Software version

    constexpr static double NORM_MAX       = 4000.;

    // IBC
    float m_IBCInvHeatsinkTemp; // [°C]
    // Additional
    int m_DCLinkPhysNom;         // [V]
    int m_PrimaryCurrentPhysNom; // [A]
    int m_TemperaturePhysNom;    // [°C]

    Version m_Version;
    SystemStatusReadings m_SysStatusReadings;
    ModuleStatusReadings m_ModStatusReadings;

    unsigned int m_SysOutVoltEnable;
    // -- Module

    // Generic ...
    unsigned int m_RemoteCtrlInp;     // namespace Remote Ctrl Inp
    double       m_DCLinkVoltageMon;  // [V]
    double       m_PrimaryCurrentMon; // [A] Tranformer primary current
    double       m_IGBTTempMon;       // [°C] heat sink of IGBT
    double       m_RectifierTempMon;  // [°C] heat sink of rectifier
    double       m_PCBTempMon;        // [°C] PCB Controller board temperature

    // Master only ...
    unsigned int m_SlopeStartupVolt;
    unsigned int m_SlopeStartupCurrent;
    unsigned int m_SlopeVolt;
    unsigned int m_SlopeCurrent;

    // Regatron
    unsigned int  m_ModuleID;
    unsigned long m_OperatingSeconds;
    unsigned long m_PowerupTimeSeconds;

    // Flash Error History
    unsigned int m_FlashErrorHistoryMaxEntries;
    std::string  ErrorHistoryEntryToString(T_ErrorHistoryEntry *entry) const;

  public:

    /** Set Module/System calls */
    void selectSys() {
        DeviceAccessControl::SelectSys();
    }

    void selectMod() {
        DeviceAccessControl::SelectMod();
    }

    // Slope
    static constexpr double SLOPE_MIN_TIME_MS = 0.05;
    static constexpr double SLOPE_MAX_TIME_MS = 1600.;
    static constexpr double SLOPE_MIN_RAW     = 1.;
    static constexpr double SLOPE_MAX_RAW     = 32000.;
    // clang-format off
    // clang-format on
    unsigned long GetOperatingSeconds();
    unsigned long GetPowerupTimeSeconds();
    double        GetSysVoltagePhysMax() const {
        return m_SysStatusReadings.GetVoltagePhysMax();
    }
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
    double GetSlopeA(const double fullScaleValue) const {
        const double MIN_VALUE_MS = fullScaleValue / SLOPE_MAX_TIME_MS;
        const double MAX_VALUE_MS = fullScaleValue / SLOPE_MIN_TIME_MS;
        return (SLOPE_MIN_RAW - SLOPE_MAX_RAW) / (MIN_VALUE_MS - MAX_VALUE_MS);
    }

    double GetSlopeB(const double fullScaleValue) const {
        const double MIN_VALUE_MS = fullScaleValue / SLOPE_MAX_TIME_MS;
        const double MAX_VALUE_MS = fullScaleValue / SLOPE_MIN_TIME_MS;
        return (SLOPE_MAX_RAW * MIN_VALUE_MS - MAX_VALUE_MS * SLOPE_MIN_RAW) /
               (MIN_VALUE_MS - MAX_VALUE_MS);
    }

    uint32_t SlopeVmsToRaw(const double voltms) const {
        return static_cast<uint32_t>(
            std::round(GetSlopeA(m_SysStatusReadings.GetVoltagePhysMax()) * voltms +
                       GetSlopeB(m_SysStatusReadings.GetVoltagePhysMax())));
    }

    unsigned int SlopeAmsToRaw(const double currentms) const {
        return static_cast<unsigned int>(
            std::round(GetSlopeA(m_SysStatusReadings.GetCurrentPhysMax()) * currentms +
                       GetSlopeB(m_SysStatusReadings.GetCurrentPhysMax())));
    }

    double SlopeRawToVms(const unsigned int raw) const {
        return (static_cast<double>(raw) - GetSlopeB(m_SysStatusReadings.GetVoltagePhysMax()) /
               GetSlopeA(m_SysStatusReadings.GetVoltagePhysMax()));
    }

    double SlopeRawToAms(const unsigned int raw) const {
        return (static_cast<double>(raw) - GetSlopeB(m_SysStatusReadings.GetCurrentPhysMax()) /
               GetSlopeA(m_SysStatusReadings.GetCurrentPhysMax()));
    }

    auto getModuleID() const { return m_ModuleID; }
    auto& getVersion() { return m_Version; }

    // -------------- Slopes -------------------
    bool SetSlopeStartupVoltMs(const double valMs);
    bool SetSlopeVoltMs(const double valMs);
    bool SetSlopeStartupVoltRaw(const double valRaw);
    bool SetSlopeVoltRaw(const double valRaw);

    bool SetSlopeStartupCurrentMs(const double valMs);
    bool SetSlopeCurrentMs(const double valMs);
    bool SetSlopeStartupCurrentRaw(const double valRaw);
    bool SetSlopeCurrentRaw(const double valRaw);

    inline double GetSlopeVoltSp() const { return SlopeRawToVms(m_SlopeVolt); }
    inline double GetSlopeStartupVoltSp() const {
        return SlopeRawToVms(m_SlopeStartupVolt);
    }
    inline double GetSlopeVoltMin() const {
        return SlopeRawToVms(static_cast<unsigned int>(SLOPE_MIN_RAW));
    }
    inline double GetSlopeVoltMax() const {
        return SlopeRawToVms(static_cast<unsigned int>(SLOPE_MAX_RAW));
    }

    inline double GetSlopeCurrentSp() const {
        return SlopeRawToAms(m_SlopeCurrent);
    }
    inline double GetSlopeStartupCurrentSp() const {
        return SlopeRawToAms(m_SlopeStartupCurrent);
    }
    inline double GetSlopeCurrentMin() const {
        return SlopeRawToAms(static_cast<unsigned int>(SLOPE_MIN_RAW));
    }
    inline double GetSlopeCurrentMax() const {
        return SlopeRawToAms(static_cast<unsigned int>(SLOPE_MAX_RAW));
    }

    bool WriteSlopeVolt();
    bool WriteSlopeCurrent();

    // -----------------------------------------

    std::string GetSlopeVolt();
    std::string GetSlopeCurrent();

    std::string getModTree();
    std::string getSysTree();

    /***
     * @return: string formatted as:
     * [m_...ActualOutVoltageMon,m_...ActualOutCurrentMon,
     * m_...ActualOutPowerMon, m_...ActualResMon, m_...State];
     */
    std::string getModReadings();
    std::string getSysReadings();

    /**
     * Read and convert to physical value DCLinkVoltage
     * @throw CommException when dll fails
     */
    void               readDCLinkVoltage();
    inline std::string getDCLinkVoltage() {
        readDCLinkVoltage();
        return fmt::format("{}", m_DCLinkVoltageMon);
    }

    /**
     * Read and convert to physical value oif transformer primary current
     * @throw CommException when dll fails
     */
    void readPrimaryCurrent();

    inline std::string getPrimaryCurrent() {
        readPrimaryCurrent();
        return fmt::format("{}", m_PrimaryCurrentMon);
    }
    /**
     * Read and convert IGBT, Rectifier and PCB temperatures.
     * @throw CommException
     */
    void readTemperature();
    /**
     * Get a string array representation of all temperatures.
     * [0] IGBT Temp
     * [1] Rect Temp
     * [2] PCB  Temp
     * @throws: CommException
     * @return string in the format "[val1,...,valn]" */
    std::string getTemperatures();

    // @todo: Restrict read/write if master ...? Here or upper layer?
    bool isMaster() const;

    /** One time readings - @throw CommException */
    void readAdditionalPhys();
    void readSystemPhys();
    void readModulePhys();
    void readModuleID();

    /** Monitor Readings @throw: CommException */
    // void readGeneric();

    /** Methods readSystem and readModule will act on the following values:
     * TC4GetVoltageActSense(&m_...ActualOutVoltageMon)
     * TC4GetPowerActSense(&m_...ActualOutPowerMon)
     * TC4GetCurrentAct(&m_...ActualOutCurrentMon)
     * TC4GetResistanceAct(&m_...ActualResMon)
     * TC4StateActSystem(&m_...State)
     */
    void readSystem();
    void readModule();

    /** Methods readSystemError32 and readModuleError32 will act on the
     * following values:
     * TC4ReadErrorTree32(&m_...ErrorTree32Mon)
     * TC4ReadWarningTree32(&m_...WarningTree32Mon)
     * */
    void readSystemErrorTree32();
    void readModuleErrorTree32();

    void         SetFlashErrorHistoryMaxEntries(unsigned int maxEntries);
    unsigned int GetFlashErrorHistoryMaxEntries() const {
        return m_FlashErrorHistoryMaxEntries;
    }
    std::string GetFlashErrorHistoryEntries() const;
    inline void storeParameters() {
        if (TC4StoreParameters() != DLL_SUCCESS) {
            throw CommException("failed to store parameters");
        }
    }

    inline void clearErrors() {
        if (TC4ClearError() != DLL_SUCCESS) {
            throw CommException("failed to clear erors");
        }
    }

    inline void readRemoteControlInput() {
        if (TC4GetRemoteControlInput(&m_RemoteCtrlInp) != DLL_SUCCESS) {
            throw CommException("failed to read remote control input");
        }
    }

    inline auto getSysControlMode() {
       return  m_SysStatusReadings.GetControlModeString();
    }

    inline auto getModControlMode() {
        return m_ModStatusReadings.GetControlModeString();
    }

    inline auto getRemoteControlInput() {
        readRemoteControlInput();
        return fmt::format("{}", m_RemoteCtrlInp);
    }

    // @todo: Check if this returns different values depending on module/system
    // (0, 64)
    double      getModCurrentRef();
    double      getModVoltageRef();
    double      getModResistanceRef();
    double      getModPowerRef();
    std::string getModMinMaxNom();

    double      getSysCurrentRef();
    double      getSysVoltageRef();
    double      getSysResistanceRef();
    double      getSysPowerRef();
    std::string getSysMinMaxNom();
    int         getSysOutVoltEnable();

    /** Calling these functions on a TopCon Slave will have no effect. */
    void setSysCurrentRef(const double);
    void setSysVoltageRef(const double);
    void setSysResistanceRef(const double);
    void setSysPowerRef(const double);
    void setSysOutVoltEnable(const unsigned int);
};
} // namespace Regatron
