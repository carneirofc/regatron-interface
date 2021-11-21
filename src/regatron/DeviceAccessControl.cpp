#include "DeviceAccessControl.hpp"

#include "fmt/format.h"
#include "serialiolib.h" // NOLINT
#include "regatron/Regatron.hpp"

namespace Regatron{
namespace DeviceAccessControl {

void SelectModuleByID(unsigned int module) {
    if (TC4SetModuleSelector(module) != DLL_SUCCESS) {
        throw CommException(fmt::format(
        "failed to set module selector to {} (code {})",
        ((module == SYS_VALUES) ? "system" : "device"), module));
    }
}

void SelectSys() {
    SelectModuleByID(SYS_VALUES);
}

void SelectMod() {
    SelectModuleByID(MOD_VALUES);
}

}}
