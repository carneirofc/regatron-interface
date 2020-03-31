#pragma once
#include <string>

namespace Regatron{
  class Version {
    private:
      /** One time readings */
      // -- Dll Version
      unsigned int m_dllMajorMinor; // (xx.68.00) and (03.xx.00)
      unsigned int m_dllBuild;      // (03.68.xx)
      std::string m_dllVerString;   // (xx.xx.xx)
      char m_dllString[128];

      // -- DSP Firwmare
      unsigned int m_DSPMain;
      unsigned int m_DSPSub;
      unsigned int m_DSPRevision;

      // -- DSP software
      unsigned int m_peripherieDSP;
      unsigned int m_peripherieAuxiliaryDSP; //  (e.g. 10, read as 0.10)
      unsigned int m_modulatorAuxiliaryDSP;  //  (e.g. 23, read as 0.24)
      unsigned int m_bootloader;             //  (e.g. 04, read as 0.04)

    public:
      void readDllVersion();
      void readDSPVersion();
  };
}