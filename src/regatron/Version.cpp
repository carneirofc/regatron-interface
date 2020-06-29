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
    LOG_INFO("Dll version: {}, {}", m_DLLVersionString, m_DLLString);
}

void Version::ReadDSPVersion() {
    // -- DSP Firwmare
    unsigned int vDSPMain;
    unsigned int vDSPSub;
    unsigned int vDSPRevision;

    if (TC4GetDeviceDSPID(&vDSPMain, &vDSPSub, &vDSPRevision) != DLL_SUCCESS) {
        throw CommException("failed to read DSP firmware version.");
    }

    m_DSPVersionString =
        fmt::format("{}.{}.{}", vDSPMain, vDSPSub, vDSPRevision);
    LOG_INFO("DSP Version: {}", m_DSPVersionString);

    unsigned short pVersionPLD{0};
    if (TC42GetFirmwareVersionPLD(&pVersionPLD) != DLL_SUCCESS) {
        throw CommException("Failed to read FirmwareVersionPLD");
    }
    m_PLDVersionString =
        fmt::format("v{:.02}", static_cast<float>(pVersionPLD) / 100.f);

    LOG_INFO("Firmware Version PLD: {}", m_PLDVersionString);

    unsigned short pVersion;
    if (TC42GetFirmwareVersionIBC(&pVersion) != DLL_SUCCESS) {
        throw CommException("Failed to read IBC Firmware Version.");
    }
    m_IBCVersionString =
        fmt::format("{:.02}", static_cast<float>(pVersion) / 100.f);
    LOG_INFO("Firmware Version IBC: {}", m_IBCVersionString);
}
} // namespace Regatron
