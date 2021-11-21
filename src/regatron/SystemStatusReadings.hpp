#pragma once

#include "StatusReadings.hpp"

namespace Regatron {

class SystemStatusReadings : public StatusReadings {
  public:
    void ReadControlMode() override;
    void ReadPhys() override;

    const char *Name() const override;

    /** Calling these functions on a TopCon Slave will have no effect. */
    void SetCurrentRef(const double);
    void SetVoltageRef(const double);
    void SetResistanceRef(const double);
    void SetPowerRef(const double);
    void SetOutVoltEnable(const uint32_t);
    int         GetOutVoltEnable();

  private:
    uint32_t m_OutVoltEnable;

  protected:
    void        Select() override;
};

} // namespace Regatron
