#include "Readings.hpp"
#include "serialiolib.h" // NOLINT

namespace Regatron {
static constexpr int   HISTORY_MAX_ENTRIES    = 300;
static constexpr float HISTORY_NANO_MILLI_CTE = 0.05F;


void Readings::readModuleID() {
    if (TC4GetModuleID(&(this->m_ModuleID)) != DLL_SUCCESS) {
        throw CommException("failed to get module ID.");
    }
}

void Readings::readSystemErrorTree32() {
    m_SysStatusReadings.ReadErrorTree32();
}

void Readings::readModuleErrorTree32() {
    m_ModStatusReadings.ReadErrorTree32();
}

void Readings::readModule() { m_ModStatusReadings.Read(); }

void Readings::readSystem() { m_SysStatusReadings.Read(); }

bool Readings::isMaster() const { return (m_ModuleID == 0); }

void Readings::readAdditionalPhys() {
    if (TC4GetAdditionalPhysicalValues(&m_DCLinkPhysNom,
                                       &m_PrimaryCurrentPhysNom,
                                       &m_TemperaturePhysNom) != DLL_SUCCESS) {
        throw CommException("failed to get additional physical values.");
    }
}

void Readings::readModulePhys() { m_ModStatusReadings.ReadPhys(); }

void Readings::readSystemPhys() { m_SysStatusReadings.ReadPhys(); }

std::string Readings::getSysMinMaxNom() {
    return m_SysStatusReadings.GetMinMaxNomString();
}
std::string Readings::getModMinMaxNom() {
    return m_ModStatusReadings.GetMinMaxNomString();
}

double Readings::getModCurrentRef() {
    return m_ModStatusReadings.GetCurrentRef();
}
double Readings::getModVoltageRef() {
    return m_ModStatusReadings.GetVoltageRef();
}
double Readings::getModResistanceRef() {
    return m_ModStatusReadings.GetResistanceRef();
}
double Readings::getModPowerRef() {
    return m_ModStatusReadings.GetPowerRef();
}

double Readings::getSysCurrentRef() {
    return m_SysStatusReadings.GetCurrentRef();
}
double Readings::getSysVoltageRef() {
    return m_SysStatusReadings.GetVoltageRef();
}
double Readings::getSysResistanceRef() {
    return m_SysStatusReadings.GetResistanceRef();
}
double Readings::getSysPowerRef() {
    return m_SysStatusReadings.GetPowerRef();
}

std::string Readings::getModTree() {
    return m_ModStatusReadings.GetErrorTreeString();
}

std::string Readings::getSysTree() {
    return m_SysStatusReadings.GetErrorTreeString();
}

/**
 * Read and convert IGBT, Rectifier and PCB temperatures.
 * @throw CommException
 */
void Readings::readTemperature() {
    int igbtTemp{0};
    int rectTemp{0};
    if (TC4GetTempDigital(&igbtTemp, &rectTemp) != DLL_SUCCESS) {
        throw CommException("failed to read IGBT and Rectifier temperature.");
    }
    if (TC42GetTemperaturePCB(&m_PCBTempMon) != DLL_SUCCESS) {
        throw CommException("failed to read PCB temperature.");
    }
    /*if (TCIBCGetInverterTemperatureHeatsink(&m_IBCInvHeatsinkTemp) !=
        DLL_SUCCESS) {
        throw CommException(
            "failed to read IBC Inverter heatsink temperature.");
    }*/
    m_IGBTTempMon = (static_cast<double>(igbtTemp) *
                     static_cast<double>(m_TemperaturePhysNom)) /
                    NORM_MAX;
    m_RectifierTempMon = (static_cast<double>(rectTemp) *
                          static_cast<double>(m_TemperaturePhysNom)) /
                         NORM_MAX;
}

/**
 * Get a string array representation of all temperatures.
 * [0] IGBT Temp
 * [1] Rect Temp
 * [2] PCB  Temp
 * @throws: CommException
 * @return string in the format "[val1,...,valn]" */
std::string Readings::getTemperatures() {
    readTemperature();
    std::ostringstream oss;
    oss << '[' << m_IGBTTempMon << ',' << m_RectifierTempMon << ','
        << m_PCBTempMon << ']';
    return oss.str();
}

std::string Readings::getModReadings() {
    return m_ModStatusReadings.GetReadingsString();
}

std::string Readings::getSysReadings() {
    return m_SysStatusReadings.GetReadingsString();
}

/**
 * Read and convert to physical value DCLinkVoltage
 * @throw CommException when dll fails
 */
void Readings::readDCLinkVoltage() {
    int DCLinkVoltStd{0};

    if (TC4GetDCLinkDigital(&DCLinkVoltStd) != DLL_SUCCESS) {
        throw CommException("failed to read DCLink digital voltage.");
    }
    m_DCLinkVoltageMon = (static_cast<double>(DCLinkVoltStd) *
                          static_cast<double>(m_DCLinkPhysNom)) /
                         NORM_MAX;
}

void Readings::readPrimaryCurrent() {
    int primaryCurrent{0};
    if (TC4GetIPrimDigital(&primaryCurrent) != DLL_SUCCESS) {
        throw CommException("failed to read transformer primary current.");
    }
    m_PrimaryCurrentMon = (static_cast<double>(primaryCurrent) *
                           static_cast<double>(m_PrimaryCurrentPhysNom)) /
                          NORM_MAX;
}

std::string
Readings::ErrorHistoryEntryToString(T_ErrorHistoryEntry *entry) const {
    return fmt::format(
        R"(T_ErrorHistoryEntry(entryCounter={},day={},hour={},minute={},second={},counter50us={},group={},detail={},identifier={}))",
        entry->entryCounter, entry->day, entry->hour, entry->minute,
        entry->second, entry->counter50us, entry->group, entry->detail,
        entry->identifier);
}

void Readings::SetFlashErrorHistoryMaxEntries(unsigned int maxEntries) {

    // @todo: Fix this harcoded limit ....
    if (maxEntries > HISTORY_MAX_ENTRIES) {
        maxEntries = HISTORY_MAX_ENTRIES;
    }

    if (maxEntries == 0) {
        maxEntries = 1;
    }
    m_FlashErrorHistoryMaxEntries = maxEntries;
}

std::string Readings::GetFlashErrorHistoryEntries() const {

    T_ErrorHistoryEntry entry{};
    unsigned int        nEntries{0};
    signed int          error{0};

    std::ostringstream oss;
    oss << '[';

    if (TC4GetFlashErrorHistorySize(&nEntries) != DLL_SUCCESS) {
        throw CommException("failed to read error history entries.");
    }
    LOG_INFO(R"(TC4ErrorHistory: Total entries: {}, Max entries read: {})",
             nEntries, m_FlashErrorHistoryMaxEntries);

    for (unsigned int nEntry = 0;
         ((nEntry < nEntries) && (nEntry < m_FlashErrorHistoryMaxEntries));
         nEntry++) {
        if (nEntry == 0) {
            if (TC4GetFlashErrorHistoryFirstEntry(&entry, &error) !=
                DLL_SUCCESS) {
                throw CommException("failed to read entry.");
            }
        } else {
            if (TC4GetFlashErrorHistoryNextEntry(&entry, &error) !=
                DLL_SUCCESS) {
                throw CommException("failed to read entry.");
            }
        }
        oss << fmt::format("{},{},{},{},{},{:.2f},{},{}", entry.entryCounter,
                           entry.day, entry.hour, entry.minute, entry.second,
                           static_cast<float>(entry.counter50us) *
                               HISTORY_NANO_MILLI_CTE,
                           entry.group, entry.detail);

        LOG_INFO("{}) {}", nEntry, ErrorHistoryEntryToString(&entry));

        oss << ' ';
    }

    oss << ']';
    return oss.str();
}

/***
 * function to read actual operating hour counter (counts seconds)
 * */
unsigned long Readings::GetOperatingSeconds() {
    if (TC4GetOperatingSeconds(&m_OperatingSeconds) != DLL_SUCCESS) {
        throw CommException("failed to read operating seconds.");
    }
    return m_OperatingSeconds;
}
/***
 * function to get operating hour counter (in seconds) at powerup
 * */
unsigned long Readings::GetPowerupTimeSeconds() {
    if (TC4GetPowerupTime(&m_PowerupTimeSeconds) != DLL_SUCCESS) {
        throw CommException("failed to read poweruptime seconds.");
    }
    return m_PowerupTimeSeconds;
}
} // namespace Regatron
