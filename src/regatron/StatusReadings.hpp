#pragma once

#include <memory>
#include <stdint.h>

#include "serialiolib.h"

#include "log/Logger.hpp"

#include "Regatron.hpp"
#include "DeviceAccessControl.hpp"

class Readings;
namespace Regatron {
constexpr static int    ERROR_TREE32_LEN = 32;
class StatusReadings {

  public:
    StatusReadings()
        : m_ErrorTree32Mon({}), m_WarningTree32Mon({}){};

    virtual ~StatusReadings() = default;

    const std::string GetErrorTreeString();
    const std::string GetMinMaxNomString() const;
    const std::string GetReadingsString();
    const std::string GetControlModeString();

    double       GetCurrentPhysMax() const { return m_CurrentPhysMax; }
    double       GetVoltagePhysMax() const { return m_VoltagePhysMax; };
    uint32_t     GetControlMode() const { return m_ControlMode; }
    virtual void ReadPhys() = 0;
    void         Read();
    void         ReadErrorTree32();
    double       GetCurrentRef();
    double       GetVoltageRef();
    double       GetResistanceRef();
    double       GetPowerRef();

    virtual const char * Name() const  = 0;
  protected:

    virtual void         ReadControlMode()     = 0;

    virtual void         Select()              = 0;
    double               m_ActualOutCurrentMon = 0;
    double               m_ActualOutPowerMon   = 0;
    double               m_ActualOutVoltageMon = 0;
    double               m_ActualResMon        = 0;
    double               m_CurrentPhysMax      = 0; // [A]
    double               m_CurrentPhysMin      = 0; // [A]
    double               m_CurrentPhysNom      = 0; // [A]
    double               m_CurrentRef          = 0;
    double               m_PowerPhysMax        = 0; // [kW]
    double               m_PowerPhysMin        = 0; // [kW]
    double               m_PowerPhysNom        = 0; // [kW]
    double               m_PowerRef            = 0;
    double               m_ResRef              = 0;
    double               m_ResistancePhysMax   = 0; // [mOhm]
    double               m_ResistancePhysMin   = 0; // [mOhm]
    double               m_ResistancePhysNom   = 0; // [mOhm]
    double               m_VoltagePhysMax      = 0; // [V]
    double               m_VoltagePhysMin      = 0; // [V]
    double               m_VoltagePhysNom      = 0; // [V]
    double               m_VoltageRef          = 0;
    struct T_ErrorTree32 m_ErrorTree32Mon;
    struct T_ErrorTree32 m_WarningTree32Mon;
    uint32_t             m_ControlMode = 0; // namespace ControlMode
    uint32_t             m_State       = 0;
};

} // namespace Regatron
