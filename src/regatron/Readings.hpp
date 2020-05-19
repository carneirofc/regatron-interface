#pragma once

#include "Version.hpp"
#include "log/Logger.hpp"
#include "serialiolib.h" // NOLINT
#include <sstream>

namespace Regatron {

constexpr unsigned int SYS_VALUES = 64;
constexpr unsigned int MOD_VALUES = 0;

namespace Slope {
constexpr double MIN_TIME = 50e-6;
constexpr double MAX_TIME = 1.6;
constexpr double MIN_RAW  = 1.;
constexpr double MAX_RAW  = 32000.;

// constexpr double B        = (MIN_TIME - MAX_RAW * MAX_TIME) / (-MAX_RAW + 1);
// constexpr double A        = MAX_TIME - B;

constexpr double SLOPE_B =
    (MIN_TIME - (MAX_RAW * MAX_TIME)) / (MIN_RAW - MAX_RAW);
constexpr double SLOPE_A = MAX_TIME - SLOPE_B;

/**
 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 32000: fastest set value ramp: 0-100% (full scale) in 50us
 @param y [5e-5 to 1.6] s
 */
unsigned int timeToRaw(double y);
/**
 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 32000: fastest set value ramp: 0-100% (full scale) in 50us
 @param x [1 32000] raw
 */
double rawToTime(double x);
} // namespace Slope

namespace State {
constexpr unsigned int POWERUP = 2;
constexpr unsigned int READY   = 4;
constexpr unsigned int RUN     = 8;
constexpr unsigned int WARNING = 10;
constexpr unsigned int ERROR   = 12;
constexpr unsigned int STOP    = 14;
} // namespace State

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
    constexpr static double NORM_MAX = 4000.;

    // IBC
    float m_IBCInvHeatsinkTemp; // [°C]
    // Additional
    int m_DCLinkPhysNom;         // [V]
    int m_PrimaryCurrentPhysNom; // [A]
    int m_TemperaturePhysNom;    // [°C]

    // System
    double m_SysVoltagePhysMax;    // [V]
    double m_SysCurrentPhysMax;    // [A]
    double m_SysPowerPhysMax;      // [kW]
    double m_SysResistancePhysMax; // [mOhm]

    double m_SysVoltagePhysMin;    // [V]
    double m_SysCurrentPhysMin;    // [A]
    double m_SysPowerPhysMin;      // [kW]
    double m_SysResistancePhysMin; // [mOhm]
    double m_SysVoltagePhysNom;    // [V]
    double m_SysCurrentPhysNom;    // [A]
    double m_SysPowerPhysNom;      // [kW]
    double m_SysResistancePhysNom; // [mOhm]
    unsigned int m_SysControlMode;       // namespace ControlMode

    struct T_ErrorTree32  m_SysErrorTree32Mon;
    struct T_ErrorTree32  m_SysWarningTree32Mon;
    unsigned int          m_SysState;
    double                m_SysActualOutVoltageMon;
    double                m_SysActualOutPowerMon;
    double                m_SysActualOutCurrentMon;
    double                m_SysActualResMon;
    double                m_SysVoltageRef;
    double                m_SysCurrentRef;
    double                m_SysPowerRef;
    double                m_SysResRef;
    unsigned int          m_SysOutVoltEnable;

    // -- Module
    double                m_ModVoltagePhysMax;    // [V]
    double                m_ModCurrentPhysMax;    // [A]
    double                m_ModPowerPhysMax;      // [kW]
    double                m_ModResistancePhysMax; // [mOhm]
    double                m_ModVoltagePhysMin;    // [V]
    double                m_ModCurrentPhysMin;    // [A]
    double                m_ModPowerPhysMin;      // [kW]
    double                m_ModResistancePhysMin; // [mOhm]
    double                m_ModVoltagePhysNom;    // [V]
    double                m_ModCurrentPhysNom;    // [A]
    double                m_ModPowerPhysNom;      // [kW]
    double                m_ModResistancePhysNom; // [mOhm]
    uint32_t              m_ModControlMode;       // namespace ControlMode
    struct T_ErrorTree32  m_ModErrorTree32Mon;
    struct T_ErrorTree32  m_ModWarningTree32Mon;
    unsigned int          m_ModState;
    double                m_ModActualOutVoltageMon;
    double                m_ModActualOutPowerMon;
    double                m_ModActualOutCurrentMon;
    double                m_ModActualResMon;
    double                m_ModVoltageRef;
    double                m_ModCurrentRef;
    double                m_ModPowerRef;
    double                m_ModResRef;

    // Generic ...
    unsigned int m_RemoteCtrlInp;     // namespace Remote Ctrl Inp
    double       m_DCLinkVoltageMon;  // [V]
    double       m_PrimaryCurrentMon; // [A] Tranformer primary current
    double       m_IGBTTempMon;       // [°C] heat sink of IGBT
    double       m_RectifierTempMon;  // [°C] heat sink of rectifier
    double       m_PCBTempMon;        // [°C] PCB Controller board temperature

    // Regatron
    unsigned int m_moduleID = 0;

  public:
    auto getModuleID() const { return m_moduleID; }
    auto getVersion() const { return m_Version; }
    /** Monitor readings */

    Readings() : m_Version(std::make_shared<Regatron::Version>()) {}

    std::string getModTree();
    std::string getSysTree();

    auto getModReadings() {
        readModule();
        return fmt::format(R"([{},{},{},{},{}])", m_ModActualOutVoltageMon,
                           m_ModActualOutCurrentMon, m_ModActualOutPowerMon,
                           m_ModActualResMon, m_ModState);
    }

    auto getSysReadings() {
        readSystem();
        return fmt::format(R"([{},{},{},{},{}])", m_SysActualOutVoltageMon,
                           m_SysActualOutCurrentMon, m_SysActualOutPowerMon,
                           m_SysActualResMon, m_SysState);
    }

    /**
     * Read and convert to physical value DCLinkVoltage
     * @throw CommException when dll fails
     */
    void readDCLinkVoltage() {
        int DCLinkVoltStd;

        if (TC4GetDCLinkDigital(&DCLinkVoltStd) != DLL_SUCCESS) {
            throw CommException("failed to read DCLink digital voltage.");
        }
        m_DCLinkVoltageMon = (DCLinkVoltStd * m_DCLinkPhysNom) / NORM_MAX;
    }
    auto getDCLinkVoltage() {
        readDCLinkVoltage();
        return fmt::format("{}", m_DCLinkVoltageMon);
    }

    void readPrimaryCurrent() {
        int primaryCurrent;
        if (TC4GetIPrimDigital(&primaryCurrent) != DLL_SUCCESS) {
            throw CommException(
                "failed to read transformer primary current.");
        }
        m_PrimaryCurrentMon =
            (primaryCurrent * m_PrimaryCurrentPhysNom) / NORM_MAX;
    }

    auto getPrimaryCurrent() {
        readPrimaryCurrent();
        return fmt::format("{}", m_PrimaryCurrentMon);
    }

    /**
     * Read and convert IGBT, Rectifier and PCB temperatures.
     * @throw CommException
     */
    void readTemperature() {
        int igbtTemp;
        int rectTemp;
        if (TC4GetTempDigital(&igbtTemp, &rectTemp) != DLL_SUCCESS) {
            throw CommException(
                "failed to read IGBT and Rectifier temperature.");
        }
        if (TC42GetTemperaturePCB(&m_PCBTempMon) != DLL_SUCCESS) {
            throw CommException("failed to read PCB temperature.");
        }
        /*if (TCIBCGetInverterTemperatureHeatsink(&m_IBCInvHeatsinkTemp) !=
            DLL_SUCCESS) {
            throw CommException(
                "failed to read IBC Inverter heatsink temperature.");
        }*/
        m_IGBTTempMon      = (igbtTemp * m_TemperaturePhysNom) / NORM_MAX;
        m_RectifierTempMon = (rectTemp * m_TemperaturePhysNom) / NORM_MAX;
    }

    /**
     * Get a string array representation of all temperatures.
     * [0] IGBT Temp
     * [1] Rect Temp
     * [2] PCB  Temp
     * @throws: CommException
     * @return string in the format "[val1,...,valn]" */
    auto getTemperatures() {
        readTemperature();
        std::ostringstream oss;
        oss << '[' << m_IGBTTempMon << ',' << m_RectifierTempMon << ','
            << m_PCBTempMon << ']';
        return oss.str();
    }

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

    /** Set Module/System calls */
    inline void selectSys() { this->selectMod(SYS_VALUES); }
    inline void selectMod() { this->selectMod(MOD_VALUES); }

    void storeParameters() {
        if (TC4StoreParameters() != DLL_SUCCESS) {
            throw CommException("failed to store parameters");
        }
    }

    void clearErrors() {
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

    auto getSysControlMode() {
        readSysControlMode();
        return fmt::format("{}", m_SysControlMode);
    }

    auto getModControlMode() {
        readModControlMode();
        return fmt::format("{}", m_ModControlMode);
    }

    auto getRemoteControlInput() {
        readRemoteControlInput();
        return fmt::format("{}", m_RemoteCtrlInp);
    }

    void selectMod(unsigned int module) {
        if (TC4SetModuleSelector(module) != DLL_SUCCESS) {
            throw CommException(fmt::format(
                "failed to set module selector to {} (code {})",
                ((module == SYS_VALUES) ? "system" : "device"), module));
        }
    }

    // @todo: Check if this returns different values depending on module/system
    // (0, 64)
    double getModCurrentRef();
    double getModVoltageRef();
    double getModResistanceRef();
    double getModPowerRef();
    std::string getModMinMaxNom();

    double getSysCurrentRef();
    double getSysVoltageRef();
    double getSysResistanceRef();
    double getSysPowerRef();
    std::string getSysMinMaxNom();
    bool   getSysOutVoltEnable();

    /** Calling these functions on a TopCon Slave will have no effect. */
    void setSysCurrentRef(double);
    void setSysVoltageRef(double);
    void setSysResistanceRef(double);
    void setSysPowerRef(double);
    void setSysOutVoltEnable(unsigned int);
};
} // namespace Regatron
