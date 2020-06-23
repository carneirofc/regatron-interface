#include "Readings.hpp"

namespace Regatron {

// ----------------------- Slope Voltage ---------------------------
bool Readings::SetSlopeVoltRaw(double dRaw) {
    auto rawVolt = static_cast<unsigned int>(dRaw);
    if (rawVolt < SLOPE_MIN_RAW || rawVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope, raw conversion is out of range "{}".)",
            rawVolt);
        return false;
    }
    m_SlopeVolt = rawVolt;
    return true;
}

bool Readings::SetSlopeStartupVoltRaw(double voltms) {
    auto rawVolt = static_cast<unsigned int>(voltms);
    if (rawVolt < SLOPE_MIN_RAW || rawVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup, raw conversion is out of range "{}".)",
            voltms, rawVolt);
        return false;
    }
    m_SlopeStartupVolt = rawVolt;
    return true;
}

bool Readings::SetSlopeVoltMs(double voltms) {
    unsigned int rawVoltMs = SlopeVmsToRaw(voltms);
    if (rawVoltMs < SLOPE_MIN_RAW || rawVoltMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope V/ms to "{}", raw conversion is out of range "{}".)",
            voltms, rawVoltMs);
        return false;
    }
    m_SlopeVolt = rawVoltMs;
    LOG_INFO(R"(Voltage Slope: {}V/ms = {})", voltms, m_SlopeVolt);
    return true;
}

bool Readings::SetSlopeStartupVoltMs(double voltms) {
    unsigned int rawVoltMs = SlopeVmsToRaw(voltms);
    if (rawVoltMs < SLOPE_MIN_RAW || rawVoltMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup slope V/ms to "{}", raw conversion is out of range "{}".)",
            voltms, rawVoltMs);
        return false;
    }
    m_SlopeStartupVolt = rawVoltMs;
    LOG_INFO(R"(Voltage Startup Slope: {}V/ms = {})", voltms, m_SlopeVolt);
    return true;
}

bool Readings::WriteSlopeVolt() {
    if (m_SlopeVolt < SLOPE_MIN_RAW || m_SlopeVolt > SLOPE_MAX_RAW ||
        m_SlopeStartupVolt < SLOPE_MIN_RAW ||
        m_SlopeStartupVolt > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(setVoltageRamp: Parameters must be >= {} and <={}.)",
            SLOPE_MIN_RAW, SLOPE_MAX_RAW);
        return false;
    }
    LOG_TRACE(R"(Configuring voltage slope to ({},{}) aka ({},{})V/ms)",
              m_SlopeStartupVolt, m_SlopeVolt,
              SlopeRawToVms(m_SlopeStartupVolt), SlopeRawToVms(m_SlopeVolt));

    LOG_CRITICAL("Not available");
    //if (TC4SetVoltageSlopeRamp(m_SlopeVolt, m_SlopeStartupVolt) !=
    //    DLL_SUCCESS) {
    //    throw CommException("Failed to set voltage slopes");
    //}
    LOG_TRACE(R"(Voltage slope results are: "{}" "{}".)", m_SlopeStartupVolt, m_SlopeVolt);
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
bool Readings::SetSlopeCurrentRaw(double raw) {
    auto rawCurrent = static_cast<unsigned int>(raw);
    if (rawCurrent < SLOPE_MIN_RAW || rawCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope, raw conversion is out of range "{}".)",
            rawCurrent);
        return false;
    }
    m_SlopeCurrent = rawCurrent;
    return true;
}

bool Readings::SetSlopeStartupCurrentRaw(double val) {
    auto rawCurrent = static_cast<unsigned int>(val);
    if (rawCurrent < SLOPE_MIN_RAW || rawCurrent > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup, raw conversion is out of range "{}".)",
            val, rawCurrent);
        return false;
    }
    m_SlopeStartupCurrent = rawCurrent;
    return true;
}

bool Readings::SetSlopeCurrentMs(double currentms) {
    unsigned int rawCurrentMs = SlopeAmsToRaw(currentms);
    if (rawCurrentMs < SLOPE_MIN_RAW || rawCurrentMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set slope A/ms to "{}", raw conversion is out of range "{}".)",
            currentms, rawCurrentMs);
        return false;
    }
    m_SlopeCurrent = rawCurrentMs;
    LOG_INFO(R"(Current Slope: {}A/ms = {})", currentms, m_SlopeCurrent);
    return true;
}

bool Readings::SetSlopeStartupCurrentMs(double currentms) {
    unsigned int rawCurrentMs = SlopeAmsToRaw(currentms);
    if (rawCurrentMs < SLOPE_MIN_RAW || rawCurrentMs > SLOPE_MAX_RAW) {
        LOG_CRITICAL(
            R"(Failed to set startup slope A/ms to "{}", raw conversion is out of range "{}".)",
            currentms, rawCurrentMs);
        return false;
    }
    m_SlopeStartupCurrent = rawCurrentMs;
    LOG_INFO(R"(Current Startup Slope: {}A/ms = {})", currentms,
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
    //if (TC4SetCurrentSlopeRamp(m_SlopeCurrent, m_SlopeStartupCurrent) !=
    //    DLL_SUCCESS) {
    //    throw CommException("Failed to set current slopes");
    //}
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
    if (TC4GetModuleID(&(this->m_moduleID)) != DLL_SUCCESS) {
        throw CommException("failed to get module ID.");
    }
}

void Readings::readModuleErrorTree32() {
    selectMod();
    if (TC4ReadErrorTree32(&m_ModErrorTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get module error tree");
    }
    if (TC4ReadWarningTree32(&m_ModWarningTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get module warn tree");
    }
}

void Readings::readModule() {

    selectMod();
    if (TC4GetVoltageAct(&m_ModActualOutVoltageMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output voltage");
    }

    if (TC4GetPowerAct(&m_ModActualOutPowerMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output power");
    }

    if (TC4GetCurrentAct(&m_ModActualOutCurrentMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual output current");
    }

    if (TC4GetResistanceAct(&m_ModActualResMon) != DLL_SUCCESS) {
        throw CommException("failed to get module actual resistence");
    }

    if (TC4StateActSystem(&m_ModState) != DLL_SUCCESS) {
        throw CommException("failed to get module state");
    }
}

void Readings::readSystemErrorTree32() {
    selectSys();
    if (TC4ReadErrorTree32(&m_SysErrorTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get system error tree");
    }
    if (TC4ReadWarningTree32(&m_SysWarningTree32Mon) != DLL_SUCCESS) {
        throw CommException("failed to get system warn tree");
    }
}

void Readings::readSystem() {
    selectSys();
    if (TC4GetVoltageAct(&m_SysActualOutVoltageMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual output voltage");
    }

    if (TC4GetPowerAct(&m_SysActualOutPowerMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual output power");
    }

    if (TC4GetCurrentAct(&m_SysActualOutCurrentMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual output current");
    }

    if (TC4GetResistanceAct(&m_SysActualResMon) != DLL_SUCCESS) {
        throw CommException("failed to get system actual resistence");
    }

    if (TC4StateActSystem(&m_SysState) != DLL_SUCCESS) {
        throw CommException("failed to get system state");
    }
}

bool Readings::isMaster() const { return (m_moduleID == 0); }

void Readings::readAdditionalPhys() {
    if (TC4GetAdditionalPhysicalValues(&m_DCLinkPhysNom,
                                       &m_PrimaryCurrentPhysNom,
                                       &m_TemperaturePhysNom) != DLL_SUCCESS) {
        throw CommException("failed to get additional physical values.");
    }
}

void Readings::readModulePhys() {
    if (TC4GetModulePhysicalLimitMax(&m_ModVoltagePhysMax, &m_ModCurrentPhysMax,
                                     &m_ModPowerPhysMax,
                                     &m_ModResistancePhysMax) != DLL_SUCCESS) {
        throw CommException(
            "failed to get module physical max limit values.");
    }

    if (TC4GetModulePhysicalLimitMin(&m_ModVoltagePhysMin, &m_ModCurrentPhysMin,
                                     &m_ModPowerPhysMin,
                                     &m_ModResistancePhysMin) != DLL_SUCCESS) {
        throw CommException(
            "failed to get module physical min limit values.");
    }

    if (TC4GetModulePhysicalLimitNom(&m_ModVoltagePhysNom, &m_ModCurrentPhysNom,
                                     &m_ModPowerPhysNom,
                                     &m_ModResistancePhysNom) != DLL_SUCCESS) {
        throw CommException(
            "failed to get module physical nominal values.");
    }
}

void Readings::readSystemPhys() {

    if (TC4GetSystemPhysicalLimitMax(&m_SysVoltagePhysMax, &m_SysCurrentPhysMax,
                                     &m_SysPowerPhysMax,
                                     &m_SysResistancePhysMax) != DLL_SUCCESS) {
        throw CommException(
            "failed to get system physical max limit values.");
    }

    if (TC4GetSystemPhysicalLimitMin(&m_SysVoltagePhysMin, &m_SysCurrentPhysMin,
                                     &m_SysPowerPhysMin,
                                     &m_SysResistancePhysMin) != DLL_SUCCESS) {
        throw CommException(
            "failed to get system physical min limit values.");
    }

    if (TC4GetSystemPhysicalLimitNom(&m_SysVoltagePhysNom, &m_SysCurrentPhysNom,
                                     &m_SysPowerPhysNom,
                                     &m_SysResistancePhysNom) != DLL_SUCCESS) {
        throw CommException(
            "failed to get system physical nominal values.");
    }
}

std::string Readings::getSysMinMaxNom() {
    return fmt::format(
        R"([{},{},{},{},{},{},{},{},{},{},{},{}])", m_SysVoltagePhysMin,
        m_SysCurrentPhysMin, m_SysPowerPhysMin, m_SysResistancePhysMin,
        m_SysVoltagePhysMax, m_SysCurrentPhysMax, m_SysPowerPhysMax,
        m_SysResistancePhysMax, m_SysVoltagePhysNom, m_SysCurrentPhysNom,
        m_SysPowerPhysNom, m_SysResistancePhysNom);
}

std::string Readings::getModMinMaxNom() {
    return fmt::format(
        R"([{},{},{},{},{},{},{},{},{},{},{},{}])", m_ModVoltagePhysMin,
        m_ModCurrentPhysMin, m_ModPowerPhysMin, m_ModResistancePhysMin,
        m_ModVoltagePhysMax, m_ModCurrentPhysMax, m_ModPowerPhysMax,
        m_ModResistancePhysMax, m_ModVoltagePhysNom, m_ModCurrentPhysNom,
        m_ModPowerPhysNom, m_ModResistancePhysNom);
}

double Readings::getModCurrentRef() {
    selectMod();
    if (TC4GetCurrentRef(&m_ModCurrentRef) != DLL_SUCCESS) {
        throw CommException("failed to read module current referece");
    }
    return m_ModCurrentRef;
}
double Readings::getModVoltageRef() {
    selectMod();
    if (TC4GetVoltageRef(&m_ModVoltageRef) != DLL_SUCCESS) {
        throw CommException("failed to read module voltage referece");
    }
    return m_ModVoltageRef;
}
double Readings::getModResistanceRef() {
    selectMod();
    if (TC4GetResistanceRef(&m_ModResRef) != DLL_SUCCESS) {
        throw CommException("failed to read module resitance referece");
    }
    return m_ModResRef;
}
double Readings::getModPowerRef() {
    selectMod();
    if (TC4GetPowerRef(&m_ModPowerRef) != DLL_SUCCESS) {
        throw CommException("failed to read module voltage referece");
    }
    return m_ModPowerRef;
}

double Readings::getSysCurrentRef() {
    selectSys();
    if (TC4GetCurrentRef(&m_SysCurrentRef) != DLL_SUCCESS) {
        throw CommException("failed to read system current referece");
    }
    return m_SysCurrentRef;
}
double Readings::getSysVoltageRef() {
    selectSys();
    if (TC4GetVoltageRef(&m_SysVoltageRef) != DLL_SUCCESS) {
        throw CommException("failed to read system voltage referece");
    }
    return m_SysVoltageRef;
}
double Readings::getSysResistanceRef() {
    selectSys();
    if (TC4GetResistanceRef(&m_SysResRef) != DLL_SUCCESS) {
        throw CommException("failed to read system resitance referece");
    }
    return m_SysResRef;
}
double Readings::getSysPowerRef() {
    selectSys();
    if (TC4GetPowerRef(&m_SysPowerRef) != DLL_SUCCESS) {
        throw CommException("failed to read system voltage referece");
    }
    return m_SysPowerRef;
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
    std::ostringstream oss;
    readModuleErrorTree32();

    // Error
    oss << '[' << m_ModErrorTree32Mon.group << ',';
    for (const auto &error : m_ModErrorTree32Mon.error) {
        oss << error;
        oss << ',';
    }

    // Warning
    int idx{0};
    oss << m_ModWarningTree32Mon.group << ',';
    for (const auto &error : m_ModWarningTree32Mon.error) {
        oss << error;
        idx++;
        if (idx < errorTree32Len) {
            oss << ',';
        }
    }
    oss << ']';
    return oss.str();
}

std::string Readings::getSysTree() {
    std::ostringstream oss;
    readSystemErrorTree32();

    // Error
    oss << '[' << m_SysErrorTree32Mon.group << ',';
    for (const auto &error : m_SysErrorTree32Mon.error) {
        oss << error;
        oss << ',';
    }

    // Warning
    int idx{0};
    oss << m_SysWarningTree32Mon.group << ',';
    for (const auto &error : m_SysWarningTree32Mon.error) {
        oss << error;
        idx++;
        if (idx < errorTree32Len) {
            oss << ',';
        }
    }
    oss << ']';
    return oss.str();
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
    m_IGBTTempMon      = (igbtTemp * m_TemperaturePhysNom) / NORM_MAX;
    m_RectifierTempMon = (rectTemp * m_TemperaturePhysNom) / NORM_MAX;
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
    readModule();
    return fmt::format(R"([{},{},{},{},{}])", m_ModActualOutVoltageMon,
                       m_ModActualOutCurrentMon, m_ModActualOutPowerMon,
                       m_ModActualResMon, m_ModState);
}

std::string Readings::getSysReadings() {
    readSystem();
    return fmt::format(R"([{},{},{},{},{}])", m_SysActualOutVoltageMon,
                       m_SysActualOutCurrentMon, m_SysActualOutPowerMon,
                       m_SysActualResMon, m_SysState);
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
    m_DCLinkVoltageMon = (DCLinkVoltStd * m_DCLinkPhysNom) / NORM_MAX;
}

void Readings::readPrimaryCurrent() {
    int primaryCurrent{0};
    if (TC4GetIPrimDigital(&primaryCurrent) != DLL_SUCCESS) {
        throw CommException("failed to read transformer primary current.");
    }
    m_PrimaryCurrentMon = (primaryCurrent * m_PrimaryCurrentPhysNom) / NORM_MAX;
}

std::string Readings::ErrorHistoryEntryToString(T_ErrorHistoryEntry *entry){
    return fmt::format(R"(T_ErrorHistoryEntry(entryCounter={},day={},hour={},minute={},second={},counter50us={},group={},detail={},identifier={}))",
        entry->entryCounter, entry->day,  entry->hour,
        entry->minute, entry->second, entry->counter50us, 
        entry->group, entry->detail, entry->identifier);
}

void Readings::SetFlashErrorHistoryMaxEntries(unsigned int maxEntries) {

    // @todo: Fix this harcoded limit ....
    if (maxEntries > 300) {
        maxEntries = 300;
    }

    if (maxEntries == 0) {
        maxEntries = 1;
    }
    m_FlashErrorHistoryMaxEntries = maxEntries;
}

std::string Readings::GetFlashErrorHistoryEntries() {

    T_ErrorHistoryEntry entry{};
    unsigned int nEntries{0};
    signed int error{0};

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
        if(nEntry == 0){
            if (TC4GetFlashErrorHistoryFirstEntry(&entry, &error) != DLL_SUCCESS) {
                throw CommException("failed to read entry.");
            }
        }else{
            if (TC4GetFlashErrorHistoryNextEntry(&entry, &error) != DLL_SUCCESS) {
                throw CommException("failed to read entry.");
            }
        }
        oss << fmt::format("{},{},{},{},{},{:.2f},{},{}", entry.entryCounter,
                           entry.day, entry.hour, entry.minute, entry.second,
                           static_cast<float>(entry.counter50us) * 0.05f,
                           entry.group, entry.detail);

        LOG_INFO("{}) {}", nEntry, ErrorHistoryEntryToString(&entry));

        //if (((nEntry + 1) != nEntries) &&
        //    ((nEntry + 1) != m_FlashErrorHistoryMaxEntries)) {
            oss << ' ';
        //}
    }

    oss << ']';
    return oss.str();
}
} // namespace Regatron
