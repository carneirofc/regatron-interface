#pragma once
#include <string>

namespace Regatron {
class Version {
  private:
    /** One time readings */
    // -- Dll Version
    unsigned int m_DLLMajorMinor; // (xx.68.00) and (03.xx.00)
    unsigned int m_DLLBuild;      // (03.68.xx)

    // -- DSP Firwmare
    unsigned int m_DSPMain;
    unsigned int m_DSPSub;
    unsigned int m_DSPRevision;

    unsigned int m_PeripherieDSPVersion;     //  (e.g. 10, read as 0.10)
    unsigned int m_ModulatorDSPVersion;      //  (e.g. 23, read as 0.24)
    unsigned int m_MainDSPBootloaderVersion; //  (e.g. 04, read as 0.04)

  public:
    std::string  m_DLLVersionString; // (xx.xx.xx)
    char         m_DLLString[128];

    std::string m_DSPVersionString;               // Main.Sub.Revision
    std::string m_PeripherieDSPVersionString;     //  (e.g. 10, read as 0.10)
    std::string m_ModulatorDSPVersionString;      //  (e.g. 23, read as 0.24)
    std::string m_MainDSPBootloaderVersionString; //  (e.g. 04, read as 0.04)

    // -- DSP software

    void readDllVersion();
    void readDSPVersion();
};
} // namespace Regatron
