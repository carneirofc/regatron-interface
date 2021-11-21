#pragma once
#include "Regatron.hpp"

#include "serialiolib.h" // NOLINT
#include "fmt/format.h"
#include "log/Logger.hpp"

#include <string>

namespace Regatron {
class Version {
  public:
    std::string  m_DLLVersionString; // (xx.xx.xx)
    char         m_DLLString[128];

    std::string m_DeviceDSPID;      // DSP Chip information
    std::string m_DSPVersionString; // Main.Sub.Revision
    std::string m_PLDVersionString; // vA.BC
    std::string m_IBCVersionString; // vA.BC
    std::string m_BootloaderVersionString; // vA.BC

    Version():
        m_DLLVersionString("undefined"),        // (xx.xx.xx)
        m_DLLString("undefined"),               // String repr
        m_DeviceDSPID("undefined"),             // DSP Chip information
        m_DSPVersionString("undefined"),        // Main.Sub.Revision
        m_PLDVersionString("undefined"),        // vA.BC
        m_IBCVersionString("undefined"),        // vA.BC
        m_BootloaderVersionString("undefined")  // vA.BC
    {}

    void ReadDllVersion();
    void ReadDSPVersion();
};
} // namespace Regatron
