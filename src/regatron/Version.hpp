#pragma once
#include "Regatron.hpp"

#include "serialiolib.h" // NOLINT
#include <fmt/format.h>
#include <log/Logger.hpp>

#include <string>

namespace Regatron {
class Version {
  private:
    /** One time readings */
    // -- Dll Version
  public:
    std::string  m_DLLVersionString; // (xx.xx.xx)
    char         m_DLLString[128];

    std::string m_DSPVersionString; // Main.Sub.Revision
    std::string m_PLDVersionString; // vA.BC
    std::string m_IBCVersionString; // vA.BC

    void ReadDllVersion();
    void ReadDSPVersion();
};
} // namespace Regatron
