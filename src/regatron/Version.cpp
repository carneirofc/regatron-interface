#include "Version.hpp"

#include "serialiolib.h" // NOLINT
#include <fmt/format.h>
#include <log/Logger.hpp>

namespace Regatron {
void Version::readDllVersion() {
    if (DllReadVersion(&m_dllMajorMinor, &m_dllBuild, m_dllString) !=
        DLL_SUCCESS) {
        throw std::runtime_error("failed to initialize tcio lib.");
    }
    m_dllVerString = fmt::format("{}.{}.{}", (m_dllMajorMinor >> 16),
                                 (m_dllMajorMinor & 0xff), m_dllBuild);
    LOG_INFO("Dll version: {}, {}", m_dllVerString, m_dllString);
}

void Version::readDSPVersion() {
    if (TC4GetDeviceDSPID(&m_DSPMain, &m_DSPSub, &m_DSPRevision) !=
        DLL_SUCCESS) {
        throw std::runtime_error("failed to read DSP firmware version.");
    }
    if (TC4GetPDSPVersion(&m_peripherieDSP) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read version of peripherie-DSP.");
    }
    if (TC4GetPeripherieVersion(&m_peripherieAuxiliaryDSP,
                                &m_modulatorAuxiliaryDSP,
                                &m_bootloader) != DLL_SUCCESS) {
        throw std::runtime_error("failed to read auxiliary DSP module version "
                                 "(peripherie, modulator and bootloader).");
    }

    LOG_INFO("Version DSP:\n"
             "  Firmware {}.{}.{}\n"
             "  Pheripherie: 0.{}\n"
             "  Bootloader: {}\n"
             "Version Auxiliary DSP:\n"
             "  Peripherie: 0.{}\n"
             "  Modulator: 0.{}",
             m_DSPMain, m_DSPSub, m_DSPRevision, m_peripherieDSP, m_bootloader,
             m_peripherieAuxiliaryDSP, m_modulatorAuxiliaryDSP);
}
} // namespace Regatron
