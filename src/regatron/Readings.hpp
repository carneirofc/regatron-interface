#pragma once

#include <cmath>
#include <memory>
#include <sstream>
#include <stdint.h>

#include "StatusReadings.hpp"
#include "ModuleStatusReadings.hpp"
#include "SystemStatusReadings.hpp"
#include "DeviceAccessControl.hpp"
#include "ControllerSettings.hpp"

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
            m_ControllerSettings(m_SysStatusReadings),

            m_IBCInvHeatsinkTemp(0),
            m_DCLinkPhysNom(0),
            m_PrimaryCurrentPhysNom(0),
            m_TemperaturePhysNom(0),

            m_RemoteCtrlInp(0),
            m_DCLinkVoltageMon(0),
            m_PrimaryCurrentMon(0),
            m_IGBTTempMon(0),
            m_RectifierTempMon(0),
            m_PCBTempMon(0),

            m_ModuleID(0),
            m_OperatingSeconds(0),
            m_PowerupTimeSeconds(0),
            m_FlashErrorHistoryMaxEntries(30)
    {}

    inline ControllerSettings &GetControllerSettings() {
        return m_ControllerSettings;
    }

    inline SystemStatusReadings &GetSystemStatus() {
        return m_SysStatusReadings;
    }

    inline ModuleStatusReadings &GetModuleStatus() {
        return m_ModStatusReadings;
    }

  private:
    Version m_Version;
    SystemStatusReadings m_SysStatusReadings;
    ModuleStatusReadings m_ModStatusReadings;
    ControllerSettings m_ControllerSettings;

    constexpr static double NORM_MAX       = 4000.;

    // IBC
    float m_IBCInvHeatsinkTemp; // [°C]
    // Additional
    int m_DCLinkPhysNom;         // [V]
    int m_PrimaryCurrentPhysNom; // [A]
    int m_TemperaturePhysNom;    // [°C]


    // -- Module

    // Generic ...
    unsigned int m_RemoteCtrlInp;     // namespace Remote Ctrl Inp
    double       m_DCLinkVoltageMon;  // [V]
    double       m_PrimaryCurrentMon; // [A] Tranformer primary current
    double       m_IGBTTempMon;       // [°C] heat sink of IGBT
    double       m_RectifierTempMon;  // [°C] heat sink of rectifier
    double       m_PCBTempMon;        // [°C] PCB Controller board temperature

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

    // clang-format off
    // clang-format on
    unsigned long GetOperatingSeconds();
    unsigned long GetPowerupTimeSeconds();
    auto getModuleID() const { return m_ModuleID; }
    auto& getVersion() { return m_Version; }

    // -----------------------------------------
    std::string getModTree();
    std::string getSysTree();

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


    inline auto getRemoteControlInput() {
        readRemoteControlInput();
        return fmt::format("{}", m_RemoteCtrlInp);
    }

};
} // namespace Regatron
