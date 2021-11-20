#pragma once
#include "StatusReadings.hpp"

namespace Regatron {
class ModuleStatusReadings : public StatusReadings {
  public:
    void ReadControlMode() override;
    void ReadPhys() override;

    const char *Name() const override;
  protected:
    void Select() override;
 
};

} // namespace Regatron