#include "Version.hpp"

namespace Regatron {
void Version::readDllVersion() {
    if (DllReadVersion(&m_DLLMajorMinor, &m_DLLBuild, m_DLLString) !=
        DLL_SUCCESS) {
        throw CommException("failed to initialize tcio lib.");
    }
    m_DLLVersionString = fmt::format("{}.{}.{}", (m_DLLMajorMinor >> 16),
                                     (m_DLLMajorMinor & 0xff), m_DLLBuild);
    LOG_INFO("Dll version: {}, {}", m_DLLVersionString, m_DLLString);
}

void Version::readDSPVersion() {

    if (TC4GetDeviceVersion(&m_DSPMain, &m_DSPSub, &m_DSPRevision) !=
        DLL_SUCCESS) {
        throw CommException("failed to read Main-DSP firmware version.");
    }

    m_DSPVersionString =
        fmt::format("{}.{}.{}", m_DSPMain, m_DSPSub, m_DSPRevision);
    LOG_INFO("DSP Version: {}", m_DSPVersionString);

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

    unsigned int aux{0};
    if (TC4GetPDSPVersion(&aux) != DLL_SUCCESS) {
        throw CommException("Failed TC4GetPDSPVersion");
    }
    LOG_INFO("TC4GetPDSPVersion: {}", aux);

    if (TC4GetPeripherieVersion(&m_PeripherieDSPVersion, &m_ModulatorDSPVersion,
                                &m_MainDSPBootloaderVersion) != DLL_SUCCESS) {
        throw CommException("failed to read auxiliary DSP module version "
                                 "(peripherie, modulator and bootloader).");
    }
    m_MainDSPBootloaderVersionString =
        fmt::format("0.{:02}", m_MainDSPBootloaderVersion);
    m_PeripherieDSPVersionString =
        fmt::format("0.{:02}", m_PeripherieDSPVersion);
    m_ModulatorDSPVersionString = fmt::format("0.{:02}", m_ModulatorDSPVersion);

    LOG_INFO("Main DSP Bootloader: {}", m_MainDSPBootloaderVersionString);
    LOG_INFO("Pheripherie DSP: {}", m_PeripherieDSPVersionString);
    LOG_INFO("Modulator DSP: {}", m_ModulatorDSPVersionString);

//DLL_RESULT TCIBCGetFirmwareVersion(unsigned short* pVersion);
//DLL_RESULT TC42GetFirmwareVersionIBC(unsigned short* pVersion);
//DLL_RESULT TC42GetFirmwareVersionPLD(unsigned short* pVersionPLD);
//DLL_RESULT TC4GetSASControlVersion(unsigned int *pversion);
//DLL_RESULT TC4GetAccuControlVersion(unsigned int *pversion);
//DLL_RESULT TC4GetTCLINInterfaceVersion(unsigned int *pinterfaceversion);
//DLL_RESULT TC4GetTCLINSystemInterfaceVersion(unsigned int *pinterfaceversion);
//DLL_RESULT TC4GetOfflinScopeVersion(unsigned int *pversion);
//DLL_RESULT TC4GetAuxilaryFunctionsInterfaceVersion(unsigned int *pinterfaceversion);
}
} // namespace Regatron
