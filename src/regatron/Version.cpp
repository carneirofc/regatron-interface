#include "Version.hpp"

#include "serialiolib.h" // NOLINT
#include <fmt/format.h>
#include <log/Logger.hpp>

namespace Regatron {
void Version::readDllVersion() {
    if (DllReadVersion(&m_DLLMajorMinor, &m_DLLBuild, m_DLLString) !=
        DLL_SUCCESS) {
        throw std::runtime_error("failed to initialize tcio lib.");
    }
    m_DLLVersionString = fmt::format("{}.{}.{}", (m_DLLMajorMinor >> 16),
                                     (m_DLLMajorMinor & 0xff), m_DLLBuild);
    LOG_INFO("Dll version: {}, {}", m_DLLVersionString, m_DLLString);
}

void Version::readDSPVersion() {
    if (TC4GetDeviceDSPID(&m_DSPMain, &m_DSPSub, &m_DSPRevision) !=
        DLL_SUCCESS) {
        throw std::runtime_error("failed to read DSP firmware version.");
    }

    if (TC4GetPeripherieVersion(&m_PeripherieDSPVersion, &m_ModulatorDSPVersion,
                                &m_MainDSPBootloaderVersion) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read auxiliary DSP module version "
                                 "(peripherie, modulator and bootloader).");
    }

    m_DSPVersionString =
        fmt::format("{}.{}.{}", m_DSPMain, m_DSPSub, m_DSPRevision);
    m_MainDSPBootloaderVersionString =
        fmt::format("0.{:02}", m_MainDSPBootloaderVersion);
    m_PeripherieDSPVersionString =
        fmt::format("0.{:02}", m_PeripherieDSPVersion);
    m_ModulatorDSPVersionString = fmt::format("0.{:02}", m_ModulatorDSPVersion);

    LOG_INFO("DSP Version: {}", m_DSPVersionString);
    LOG_INFO("Main DSP Bootloader: {}", m_MainDSPBootloaderVersionString);
    LOG_INFO("Pheripherie DSP: {}", m_PeripherieDSPVersionString);
    LOG_INFO("Modulator DSP: {}", m_ModulatorDSPVersionString);
}
} // namespace Regatron
