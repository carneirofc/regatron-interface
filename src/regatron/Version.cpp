#include "Version.hpp"

namespace Regatron {
void Version::ReadDllVersion() {
    unsigned int pDLLMajorMinor{0}; // (xx.68.00) and (03.xx.00)
    unsigned int pDLLBuild{0};      // (03.68.xx)

    if (DllReadVersion(&pDLLMajorMinor, &pDLLBuild, m_DLLString) !=
        DLL_SUCCESS) {
        throw CommException("failed to initialize tcio lib.");
    }
    m_DLLVersionString = fmt::format("{}.{}.{}", (pDLLMajorMinor >> 16),
                                     (pDLLMajorMinor & 0xff), pDLLBuild);
    LOG_INFO("DLL version: {}, {}", m_DLLVersionString, m_DLLString);
}

void Version::ReadDSPVersion() {
    // ------------- DSP Chip ------------
    unsigned int pChipID{0};
    unsigned int pChipRev{0};
    unsigned int pChipSubID{0};

    if (TC4GetDeviceDSPID(&pChipID, &pChipRev, &pChipSubID) !=
        DLL_SUCCESS) {
        throw CommException("failed to read DSP ID.");
    }
    m_DeviceDSPID = fmt::format("ChipID: {} ChipRev: {} ChipSubID: {}", pChipID,
            pChipRev, pChipSubID);
    LOG_INFO(m_DeviceDSPID);

    // ------------- DSP Firwmare ------------
    unsigned int vDSPMain{0};
    unsigned int vDSPSub{0};
    unsigned int vDSPRevision{0};

    if (TC4GetDeviceVersion(&vDSPMain, &vDSPSub, &vDSPRevision) !=
        DLL_SUCCESS) {
        throw CommException("failed to read Main-DSP firmware version.");
    }

    m_DSPVersionString =
        fmt::format("V{}.{}.{}", vDSPMain, vDSPSub, vDSPRevision);
    LOG_INFO("DSP Version (Main): {}", m_DSPVersionString);

    // ------------- Bootloader ----------
    /** 
     * According to the official support, this function is not compatible with version 4.20.
     * Bootloader version still match the proprietary software and It seems that this is the only way to read it.
     * */
    unsigned int pVersionPeripherieDSP{0};
    unsigned int pVersionModulatorDSP{0};
    unsigned int pVersionBootloader{0};
    if (TC4GetPeripherieVersion(&pVersionPeripherieDSP, &pVersionModulatorDSP,
                                   &pVersionBootloader)!= DLL_SUCCESS){
        throw CommException("failed to read bootloader version.");
    }
    m_BootloaderVersionString = 
        fmt::format("V{:0.2f}", static_cast<float>(pVersionBootloader) / 100.f);
    LOG_INFO("Bootloader Version (Main): {}", m_BootloaderVersionString);

    // ------------- PLD Firmware Version ------------
    unsigned short pVersionPLD{0};
    if (TC42GetFirmwareVersionPLD(&pVersionPLD) != DLL_SUCCESS) {
        throw CommException("Failed to read FirmwareVersionPLD");
    }
    m_PLDVersionString =
        fmt::format("V{:0.2f}", static_cast<float>(pVersionPLD) / 100.f);

    LOG_INFO("Firmware Version PLD: {}", m_PLDVersionString);

    // ------------- IBC Firmware Version ------------
    unsigned short pVersion;
    if (TC42GetFirmwareVersionIBC(&pVersion) != DLL_SUCCESS) {
        throw CommException("Failed to read IBC Firmware Version.");
    }
    if (pVersion != 0){
        m_IBCVersionString =
            fmt::format("V{:0.2f}", static_cast<float>(pVersion) / 100.f);
    } else {
        m_IBCVersionString = "not_available";
    }
    LOG_INFO("Firmware Version IBC: {}", m_IBCVersionString);
}
} // namespace Regatron
