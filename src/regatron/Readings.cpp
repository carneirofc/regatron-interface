#include "Readings.hpp"
#include "serialiolib.h" // NOLINT

namespace Regatron {
static constexpr int   HISTORY_MAX_ENTRIES    = 300;
static constexpr float HISTORY_NANO_MILLI_CTE = 0.05F;

// ----------------------- Slope Voltage ---------------------------
bool Readings::SetSlopeVoltRaw(const double valRaw) {
    const auto rawVolt = static_cast<unsigned int>(valRaw);
    if (rawVolt < SLOPE_MIN_RAW || rawVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set voltage slope, "{}" is out of range "{}"<=x<="{}.)",
            rawVolt, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeVolt = rawVolt;
    return true;
}

bool Readings::SetSlopeStartupVoltRaw(const double valRaw) {
    const auto rawVolt = static_cast<unsigned int>(valRaw);
    if (rawVolt < SLOPE_MIN_RAW || rawVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup voltage slope, "{}" is out of range "{}"<=x<="{}.)",
            rawVolt, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeStartupVolt = rawVolt;
    return true;
}

bool Readings::SetSlopeVoltMs(const double valMs) {
    const unsigned int rawVoltMs = SlopeVmsToRaw(valMs);
    if (rawVoltMs < SLOPE_MIN_RAW || rawVoltMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope V/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawVoltMs);
        return false;
    }
    m_SlopeVolt = rawVoltMs;
    LOG_INFO(R"(Voltage Slope: {}V/ms = {})", valMs, m_SlopeVolt);
    return true;
}

bool Readings::SetSlopeStartupVoltMs(const double valMs) {
    const unsigned int rawVoltMs = SlopeVmsToRaw(valMs);
    if (rawVoltMs < SLOPE_MIN_RAW || rawVoltMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup slope V/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawVoltMs);
        return false;
    }
    m_SlopeStartupVolt = rawVoltMs;
    LOG_INFO(R"(Voltage Startup Slope: {}V/ms = {})", valMs, m_SlopeVolt);
    return true;
}

bool Readings::WriteSlopeVolt() {
    if (m_SlopeVolt < SLOPE_MIN_RAW || m_SlopeVolt > SLOPE_MAX_RAW ||
        m_SlopeStartupVolt < SLOPE_MIN_RAW ||
        m_SlopeStartupVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(R"(setVoltageRamp: Parameters must be >= {} and <={}.)",
                     SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    LOG_TRACE(R"(Configuring voltage slope to ({},{}) aka ({},{})V/ms)",
              m_SlopeStartupVolt, m_SlopeVolt,
              SlopeRawToVms(m_SlopeStartupVolt), SlopeRawToVms(m_SlopeVolt));

    LOG_CRITICAL("Not available");
    if (TC4SetVoltageSlopeRamp(m_SlopeVolt, m_SlopeStartupVolt) !=
        DLL_SUCCESS) {
        throw CommException("Failed to set voltage slopes");
    }
    LOG_TRACE(R"(Voltage slope results are: "{}" "{}".)", m_SlopeStartupVolt,
              m_SlopeVolt);
    return true;
}

/**
 * @return: String formatted as an array containing:
 * [rawStartup,raw,V/ms statup,V/ms]
 * */
std::string Readings::GetSlopeVolt() {
    unsigned int startupValue{};
    unsigned int value{};
    if (TC4GetVoltageSlopeRamp(&value, &startupValue) != DLL_SUCCESS) {
        throw CommException("failed to get voltage slope ramp values.");
    }
    return fmt::format("[{},{},{},{}]", startupValue, value,
                       SlopeRawToVms(startupValue), SlopeRawToVms(value));
}

// ----------------------- Slope Current ---------------------------
bool Readings::SetSlopeCurrentRaw(const double valRaw) {
    const auto rawCurrent = static_cast<unsigned int>(valRaw);
    if (rawCurrent < SLOPE_MIN_RAW || rawCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set current slope, "{}" is out of range "{}"<=x<="{}.)",
            rawCurrent, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeCurrent = rawCurrent;
    return true;
}

bool Readings::SetSlopeStartupCurrentRaw(const double valRaw) {
    const auto rawCurrent = static_cast<unsigned int>(valRaw);
    if (rawCurrent < SLOPE_MIN_RAW || rawCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set current startup slope, "{}" is out of range "{}"<=x<="{}.)",
            rawCurrent, SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    m_SlopeStartupCurrent = rawCurrent;
    return true;
}

bool Readings::SetSlopeCurrentMs(const double valMs) {
    const unsigned int rawCurrentMs = SlopeAmsToRaw(valMs);
    if (rawCurrentMs < SLOPE_MIN_RAW || rawCurrentMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope A/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawCurrentMs);
        return false;
    }
    m_SlopeCurrent = rawCurrentMs;
    LOG_INFO(R"(Current Slope: {}A/ms = {})", valMs, m_SlopeCurrent);
    return true;
}

bool Readings::SetSlopeStartupCurrentMs(const double valMs) {
    const unsigned int rawCurrentMs = SlopeAmsToRaw(valMs);
    if (rawCurrentMs < SLOPE_MIN_RAW || rawCurrentMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup slope A/ms to "{}", raw conversion is out of range "{}".)",
            valMs, rawCurrentMs);
        return false;
    }
    m_SlopeStartupCurrent = rawCurrentMs;
    LOG_INFO(R"(Current Startup Slope: {}A/ms = {})", valMs,
             m_SlopeStartupCurrent);
    return true;
}

bool Readings::WriteSlopeCurrent() {
    if (m_SlopeCurrent < SLOPE_MIN_RAW || m_SlopeCurrent > SLOPE_MAX_RAW ||
        m_SlopeStartupCurrent < SLOPE_MIN_RAW ||
        m_SlopeStartupCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(setCurrentRamp: Parameters "{}" and "{}" must be >= {} and <={}.)",
            m_SlopeStartupCurrent, m_SlopeCurrent, SLOPE_MIN_RAW,
            SLOPE_MAX_RAW);
        return false;
    }
    LOG_TRACE(R"(Configuring current slope to ({},{}) aka ({},{})A/ms)",
              m_SlopeStartupCurrent, m_SlopeCurrent,
              SlopeRawToAms(m_SlopeStartupCurrent),
              SlopeRawToAms(m_SlopeCurrent));

    LOG_CRITICAL("Not available");
    if (TC4SetCurrentSlopeRamp(m_SlopeCurrent, m_SlopeStartupCurrent) !=
        DLL_SUCCESS) {
        throw CommException("Failed to set current slopes");
    }
    LOG_TRACE(R"(Currentage slope results are: "{}" "{}".)",
              m_SlopeStartupCurrent, m_SlopeCurrent);
    return true;
}

/**
 * @return: String formatted as an array containing:
 * [rawStartup,raw,A/ms statup,A/ms]
 * */
std::string Readings::GetSlopeCurrent() {
    unsigned int startupValue{};
    unsigned int value{};
    if (TC4GetCurrentSlopeRamp(&value, &startupValue) != DLL_SUCCESS) {
        throw CommException("failed to get current slope ramp values.");
    }
    return fmt::format("[{},{},{},{}]", startupValue, value,
                       SlopeRawToAms(startupValue), SlopeRawToAms(value));
}

// -----------------------------------------------------------------------

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

void Readings::setSysCurrentRef(double value /* [A] */) {
    selectSys();
    if (TC4SetCurrentRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system current referece");
    }
}
void Readings::setSysVoltageRef(double value /* [V] */) {
    selectSys();
    if (TC4SetVoltageRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system voltage referece");
    }
}
void Readings::setSysPowerRef(double value /* [kW] */) {
    selectSys();
    if (TC4SetPowerRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system power referece");
    }
}
void Readings::setSysResistanceRef(double value /* [mOhm] */) {
    selectSys();
    if (TC4SetResistanceRef(value) != DLL_SUCCESS) {
        throw CommException("failed to set system resistance referece");
    }
}

std::string Readings::getModTree() {
    return m_ModStatusReadings.GetErrorTreeString();
}

std::string Readings::getSysTree() {
    return m_SysStatusReadings.GetErrorTreeString();
}

void Readings::setSysOutVoltEnable(unsigned int state) {
    if (TC4SetControlIn(state) != DLL_SUCCESS) {
        throw CommException("failed to set system output voltage state");
    }
}

int Readings::getSysOutVoltEnable() {
    if (TC4GetControlIn(&m_SysOutVoltEnable) != DLL_SUCCESS) {
        throw CommException("failed to get system output voltage state");
    }
    return static_cast<int>(m_SysOutVoltEnable);
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
