#pragma once

namespace Regatron::DeviceAccessControl {

    constexpr unsigned int SYS_VALUES = 64;
    constexpr unsigned int MOD_VALUES = 0;

    void SelectModuleByID(unsigned int module);
    void SelectSys();
    void SelectMod();
}
