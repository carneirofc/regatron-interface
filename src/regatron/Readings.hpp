#pragma once

#include "Version.hpp"
#include "log/Logger.hpp"
#include "serialiolib.h" // NOLINT
#include <cmath>
#include <sstream>

namespace Regatron {

constexpr unsigned int SYS_VALUES = 64;
constexpr unsigned int MOD_VALUES = 0;

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

namespace RemoteCtrlInp {
/*
   0: set active interface to analog
   1: set active interface to HMI/HME
   2: set active interface to RS232 (this API)
   32767: no active interface will be selected
*/

}
class Readings {
  private:
    /** One time readings */

    // Software version
    std::shared_ptr<Regatron::Version> m_Version;

    constexpr static int    errorTree32Len = 32;
    constexpr static double NORM_MAX       = 4000.;

    // IBC
    float m_IBCInvHeatsinkTemp; // [°C]
    // Additional
    int m_DCLinkPhysNom;         // [V]
    int m_PrimaryCurrentPhysNom; // [A]
    int m_TemperaturePhysNom;    // [°C]

    // System
    double               m_SysVoltagePhysMax;    // [V]
    double               m_SysCurrentPhysMax;    // [A]
    double               m_SysPowerPhysMax;      // [kW]
    double               m_SysResistancePhysMax; // [mOhm]
    double               m_SysVoltagePhysMin;    // [V]
    double               m_SysCurrentPhysMin;    // [A]
    double               m_SysPowerPhysMin;      // [kW]
    double               m_SysResistancePhysMin; // [mOhm]
    double               m_SysVoltagePhysNom;    // [V]
    double               m_SysCurrentPhysNom;    // [A]
    double               m_SysPowerPhysNom;      // [kW]
    double               m_SysResistancePhysNom; // [mOhm]
    unsigned int         m_SysControlMode;       // namespace ControlMode
    struct T_ErrorTree32 m_SysErrorTree32Mon;
    struct T_ErrorTree32 m_SysWarningTree32Mon;
    unsigned int         m_SysState;
    double               m_SysActualOutVoltageMon;
    double               m_SysActualOutPowerMon;
    double               m_SysActualOutCurrentMon;
    double               m_SysActualResMon;
    double               m_SysVoltageRef;
    double               m_SysCurrentRef;
    double               m_SysPowerRef;
    double               m_SysResRef;
    unsigned int         m_SysOutVoltEnable;

    // -- Module
    double               m_ModVoltagePhysMax;    // [V]
    double               m_ModCurrentPhysMax;    // [A]
    double               m_ModPowerPhysMax;      // [kW]
    double               m_ModResistancePhysMax; // [mOhm]
    double               m_ModVoltagePhysMin;    // [V]
    double               m_ModCurrentPhysMin;    // [A]
    double               m_ModPowerPhysMin;      // [kW]
    double               m_ModResistancePhysMin; // [mOhm]
    double               m_ModVoltagePhysNom;    // [V]
    double               m_ModCurrentPhysNom;    // [A]
    double               m_ModPowerPhysNom;      // [kW]
    double               m_ModResistancePhysNom; // [mOhm]
    uint32_t             m_ModControlMode;       // namespace ControlMode
    struct T_ErrorTree32 m_ModErrorTree32Mon;
    struct T_ErrorTree32 m_ModWarningTree32Mon;
    unsigned int         m_ModState;
    double               m_ModActualOutVoltageMon;
    double               m_ModActualOutPowerMon;
    double               m_ModActualOutCurrentMon;
    double               m_ModActualResMon;
    double               m_ModVoltageRef;
    double               m_ModCurrentRef;
    double               m_ModPowerRef;
    double               m_ModResRef;

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
    unsigned int  m_moduleID;
    unsigned long m_OperatingSeconds;
    unsigned long m_PowerupTimeSeconds;

    // Flash Error History
    unsigned int m_FlashErrorHistoryMaxEntries;
    std::string  ErrorHistoryEntryToString(T_ErrorHistoryEntry *entry);

  public:
    // Slope
    static constexpr double SLOPE_MIN_TIME_MS = 0.05;
    static constexpr double SLOPE_MAX_TIME_MS = 1600.;
    static constexpr double SLOPE_MIN_RAW     = 1.;
    static constexpr double SLOPE_MAX_RAW     = 32000.;
    // clang-format off
    Readings()
        : m_Version(std::make_shared<Regatron::Version>()),
            m_IBCInvHeatsinkTemp(0),
            m_DCLinkPhysNom(0),
            m_PrimaryCurrentPhysNom(0),
            m_TemperaturePhysNom(0),

            m_SysVoltagePhysMax(0),
            m_SysCurrentPhysMax(0),
            m_SysPowerPhysMax(0),
            m_SysResistancePhysMax(0),
            m_SysVoltagePhysMin(0),
            m_SysCurrentPhysMin(0),
            m_SysPowerPhysMin(0),
            m_SysResistancePhysMin(0),
            m_SysVoltagePhysNom(0),
            m_SysCurrentPhysNom(0),
            m_SysPowerPhysNom(0),
            m_SysResistancePhysNom(0),
            m_SysControlMode(0),
            m_SysErrorTree32Mon({}),
            m_SysWarningTree32Mon({}),
            m_SysState(0),
            m_SysActualOutVoltageMon(0),
            m_SysActualOutPowerMon(0),
            m_SysActualOutCurrentMon(0),
            m_SysActualResMon(0),
            m_SysVoltageRef(0),
            m_SysCurrentRef(0),
            m_SysPowerRef(0),
            m_SysResRef(0),
            m_SysOutVoltEnable(0),

            m_ModVoltagePhysMax(0),
            m_ModCurrentPhysMax(0),
            m_ModPowerPhysMax(0),
            m_ModResistancePhysMax(0),
            m_ModVoltagePhysMin(0),
            m_ModCurrentPhysMin(0),
            m_ModPowerPhysMin(0),
            m_ModResistancePhysMin(0),
            m_ModVoltagePhysNom(0),
            m_ModCurrentPhysNom(0),
            m_ModPowerPhysNom(0),
            m_ModResistancePhysNom(0),
            m_ModControlMode(0),
            m_ModErrorTree32Mon({}),
            m_ModWarningTree32Mon({}),
            m_ModState(0),
            m_ModActualOutVoltageMon(0),
            m_ModActualOutPowerMon(0),
            m_ModActualOutCurrentMon(0),
            m_ModActualResMon(0),
            m_ModVoltageRef(0),
            m_ModCurrentRef(0),
            m_ModPowerRef(0),
            m_ModResRef(0),

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

            m_moduleID(0),
            m_OperatingSeconds(0),
            m_PowerupTimeSeconds(0),
            m_FlashErrorHistoryMaxEntries(30)
    {}
    // clang-format on
    unsigned long GetOperatingSeconds();
    unsigned long GetPowerupTimeSeconds();
    double        GetSysVoltagePhysMax() const { return m_SysVoltagePhysMax; }
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
    double GetSlopeA(double fullScaleValue) {
        const double MIN_VALUE_MS = fullScaleValue / SLOPE_MAX_TIME_MS;
        const double MAX_VALUE_MS = fullScaleValue / SLOPE_MIN_TIME_MS;
        const double a =
            (SLOPE_MIN_RAW - SLOPE_MAX_RAW) / (MIN_VALUE_MS - MAX_VALUE_MS);
        return a;
    }

    double GetSlopeB(double fullScaleValue) {
        const double MIN_VALUE_MS = fullScaleValue / SLOPE_MAX_TIME_MS;
        const double MAX_VALUE_MS = fullScaleValue / SLOPE_MIN_TIME_MS;
        const double b =
            (SLOPE_MAX_RAW * MIN_VALUE_MS - MAX_VALUE_MS * SLOPE_MIN_RAW) /
            (MIN_VALUE_MS - MAX_VALUE_MS);
        return b;
    }

    unsigned int SlopeVmsToRaw(double voltms) {
        unsigned int response = static_cast<unsigned int>(
            std::round(GetSlopeA(m_SysVoltagePhysMax) * voltms +
                       GetSlopeB(m_SysVoltagePhysMax)));
        return response;
    }

    unsigned int SlopeAmsToRaw(double currentms) {
        unsigned int response = static_cast<unsigned int>(
            std::round(GetSlopeA(m_SysCurrentPhysMax) * currentms +
                       GetSlopeB(m_SysCurrentPhysMax)));
        return response;
    }

    double SlopeRawToVms(unsigned int raw) {
        double response =
            (static_cast<double>(raw) - GetSlopeB(m_SysVoltagePhysMax)) /
            GetSlopeA(m_SysVoltagePhysMax);
        return response;
    }

    double SlopeRawToAms(unsigned int raw) {
        double response =
            (static_cast<double>(raw) - GetSlopeB(m_SysCurrentPhysMax)) /
            GetSlopeA(m_SysCurrentPhysMax);
        return response;
    }

    auto getModuleID() const { return m_moduleID; }
    auto getVersion() const { return m_Version; }

    // -------------- Slopes -------------------
    bool SetSlopeStartupVoltMs(double valMs);
    bool SetSlopeVoltMs(double valMs);
    bool SetSlopeStartupVoltRaw(double valRaw);
    bool SetSlopeVoltRaw(double valRaw);

    bool SetSlopeStartupCurrentMs(double valMs);
    bool SetSlopeCurrentMs(double valMs);
    bool SetSlopeStartupCurrentRaw(double valRaw);
    bool SetSlopeCurrentRaw(double valRaw);

    inline double GetSlopeVoltSp() { return SlopeRawToVms(m_SlopeVolt); }
    inline double GetSlopeStartupVoltSp() {
        return SlopeRawToVms(m_SlopeStartupVolt);
    }
    inline double GetSlopeVoltMin() {
        return SlopeRawToVms(static_cast<unsigned int>(SLOPE_MIN_RAW));
    }
    inline double GetSlopeVoltMax() {
        return SlopeRawToVms(static_cast<unsigned int>(SLOPE_MAX_RAW));
    }

    inline double GetSlopeCurrentSp() { return SlopeRawToAms(m_SlopeCurrent); }
    inline double GetSlopeStartupCurrentSp() {
        return SlopeRawToAms(m_SlopeStartupCurrent);
    }
    inline double GetSlopeCurrentMin() {
        return SlopeRawToAms(static_cast<unsigned int>(SLOPE_MIN_RAW));
    }
    inline double GetSlopeCurrentMax() {
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

    void   SetFlashErrorHistoryMaxEntries(unsigned int maxEntries);
    double GetFlashErrorHistoryMaxEntries() {
        return m_FlashErrorHistoryMaxEntries;
    }
    std::string GetFlashErrorHistoryEntries();

    /** Set Module/System calls */
    inline void selectSys() { this->selectMod(SYS_VALUES); }
    inline void selectMod() { this->selectMod(MOD_VALUES); }

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

    inline void readModControlMode() {
        selectMod();
        if (TC4GetControlMode(&m_ModControlMode) != DLL_SUCCESS) {
            throw CommException("failed to read module control mode");
        }
        selectSys();
    }

    inline void readSysControlMode() {
        selectSys();
        if (TC4GetControlMode(&m_SysControlMode) != DLL_SUCCESS) {
            throw CommException("failed to read system control mode");
        }
    }

    inline void readRemoteControlInput() {
        if (TC4GetRemoteControlInput(&m_RemoteCtrlInp) != DLL_SUCCESS) {
            throw CommException("failed to read remote control input");
        }
    }

    inline auto getSysControlMode() {
        readSysControlMode();
        return fmt::format("{}", m_SysControlMode);
    }

    inline auto getModControlMode() {
        readModControlMode();
        return fmt::format("{}", m_ModControlMode);
    }

    inline auto getRemoteControlInput() {
        readRemoteControlInput();
        return fmt::format("{}", m_RemoteCtrlInp);
    }

    inline void selectMod(unsigned int module) {
        if (TC4SetModuleSelector(module) != DLL_SUCCESS) {
            throw CommException(fmt::format(
                "failed to set module selector to {} (code {})",
                ((module == SYS_VALUES) ? "system" : "device"), module));
        }
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
    void setSysCurrentRef(double);
    void setSysVoltageRef(double);
    void setSysResistanceRef(double);
    void setSysPowerRef(double);
    void setSysOutVoltEnable(unsigned int);
};
} // namespace Regatron
