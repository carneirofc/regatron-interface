
#include "Handler.hpp"

namespace Regatron {
#define SET_FUNC_UINT(func)                                                    \
    [this](double arg1) {                                                      \
        auto readings = this->m_RegatronComm->getReadings();                   \
        if (readings) {                                                        \
            readings.value()->func(static_cast<unsigned int>(arg1));           \
            return ACK;                                                        \
        }                                                                      \
        return NACK;                                                           \
    }

#define SET_FUNC_DOUBLE(func)                                                  \
    [this](double arg1) {                                                      \
        auto readings = this->m_RegatronComm->getReadings();                   \
        if (readings) {                                                        \
            readings.value()->func(arg1);                                      \
            return ACK;                                                        \
        }                                                                      \
        return NACK;                                                           \
    }

#define GET_FUNC(func)                                                         \
    [this]() {                                                                 \
        auto readings = this->m_RegatronComm->getReadings();                   \
        return readings ? readings.value()->func : NACK;                       \
    }

#define GET_FORMAT(member)                                                     \
    [this]() {                                                                 \
        auto readings = this->m_RegatronComm->getReadings();                   \
        return readings ? fmt::format("{}", readings.value()->member) : NACK;  \
    }

#define CMD_API(member)                                                        \
    [this]() {                                                                 \
        auto readings = this->m_RegatronComm->getReadings();                   \
        if (readings) {                                                        \
            readings.value()->member();                                        \
            return ACK;                                                        \
        }                                                                      \
        return NACK;                                                           \
    }

volatile static double debugValue{0.0};

// @fixme: Do this in a way that does not require macros.
Handler::Handler(std::shared_ptr<Regatron::Comm> regatronComm)
    : m_RegatronComm(regatronComm),
      m_Matchers({
          // clang-format off
          Match{"getDebug", [this](){ return fmt::format("{}", debugValue); }},
          Match{"setDebug", [this](double value){ debugValue = value; return ACK; }},

          Match{"cmdConnect", [this](){ return (this->m_RegatronComm->connect()) ? ACK : NACK; }},
          Match{"cmdDisconnect", [this](){ this->m_RegatronComm->disconnect(); return ACK;}},
          Match{"getCommStatus", [this](){ return fmt::format("{}", this->m_RegatronComm->getCommStatus()); }},
          Match{"getAutoReconnect", [this](){ return fmt::format("{}", static_cast<int>(this->m_RegatronComm->getAutoReconnect())); }},
          Match{"setAutoReconnect", [this](float autoReconnect){ this->m_RegatronComm->setAutoReconnect(autoReconnect != 0); return ACK; }},

          Match{"getFlashErrorHistory",         GET_FUNC(GetFlashErrorHistoryEntries())},
          Match{"setFlashErrorHistoryMax",      SET_FUNC_UINT(SetFlashErrorHistoryMaxEntries)},
          Match{"getFlashErrorHistoryMax",      GET_FORMAT(GetFlashErrorHistoryMaxEntries())},
          Match{"getOperatingSeconds",          GET_FORMAT(GetOperatingSeconds())},
          Match{"getPowerupTimeSeconds",        GET_FORMAT(GetPowerupTimeSeconds())},

          // Commands with no response
          Match{"cmdStoreParam",                CMD_API(storeParameters)},
          Match{"cmdClearErrors",               CMD_API(clearErrors)},

          // Simple readings
          Match{"getModuleID",                  GET_FORMAT(getModuleID())},

          Match{"getDSPID",                     GET_FUNC(getVersion().m_DeviceDSPID)},
          Match{"getDSPVersion",                GET_FUNC(getVersion().m_DSPVersionString)},
          Match{"getDLLVersion",                GET_FUNC(getVersion().m_DLLVersionString)},
          Match{"getPLDVersion",                GET_FUNC(getVersion().m_PLDVersionString)},
          Match{"getIBCVersion",                GET_FUNC(getVersion().m_IBCVersionString)},
          Match{"getBootloaderVersion",         GET_FUNC(getVersion().m_BootloaderVersionString)},

          Match{"getDCLinkVoltage",             GET_FUNC(getDCLinkVoltage())},
          Match{"getPrimaryCurrent",            GET_FUNC(getPrimaryCurrent())},

          Match{"getControlInput",              GET_FUNC(getRemoteControlInput())},
          Match{"getModControlMode",            GET_FUNC(getModControlMode())},
          Match{"getModMinMaxNom",              GET_FUNC(getModMinMaxNom())},
          Match{"getModReadings",               GET_FUNC(getModReadings())},
          Match{"getSysControlMode",            GET_FUNC(getSysControlMode())},
          Match{"getSysMinMaxNom",              GET_FUNC(getSysMinMaxNom())},
          Match{"getSysReadings",               GET_FUNC(getSysReadings())},
          Match{"getTemperatures",              GET_FUNC(getTemperatures())},

          // Error + Warning T_ErrorTree32
          Match{"getModTree",                   GET_FUNC(getModTree())},
          Match{"getSysTree",                   GET_FUNC(getSysTree())},

          Match{"getModCurrentRef",             GET_FORMAT(getModCurrentRef())},
          Match{"getModVoltageRef",             GET_FORMAT(getModVoltageRef())},
          Match{"getModResistanceRef",          GET_FORMAT(getModResistanceRef())},
          Match{"getModPowerRef",               GET_FORMAT(getModPowerRef())},

          Match{"getSysCurrentRef",             GET_FORMAT(getSysCurrentRef())},
          Match{"getSysVoltageRef",             GET_FORMAT(getSysVoltageRef())},
          Match{"getSysResistanceRef",          GET_FORMAT(getSysResistanceRef())},
          Match{"getSysPowerRef",               GET_FORMAT(getSysPowerRef())},
          Match{"getSysOutVoltEnable",          GET_FORMAT(GetSystemStatus().GetOutVoltEnable())},

          Match{"setSysCurrentRef",             SET_FUNC_DOUBLE(GetSystemStatus().SetCurrentRef)},
          Match{"setSysVoltageRef",             SET_FUNC_DOUBLE(GetSystemStatus().SetVoltageRef)},
          Match{"setSysResistanceRef",          SET_FUNC_DOUBLE(GetSystemStatus().SetResistanceRef)},
          Match{"setSysPowerRef",               SET_FUNC_DOUBLE(GetSystemStatus().SetPowerRef)},
          Match{"setSysOutVoltEnable",          SET_FUNC_UINT(GetSystemStatus().SetOutVoltEnable)},

          /*** Calling this on slaves will have no effect */
          // Slopes Voltage
          Match{"setSlopeVoltMs",               SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeVoltMs)},
          Match{"setSlopeStartupVoltMs",        SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeStartupVoltMs)},
          Match{"setSlopeVoltRaw",              SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeVoltRaw)},
          Match{"setSlopeStartupVoltRaw",       SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeStartupVoltRaw)},

          Match{"cmdSlopeVoltWrite",            CMD_API(GetControllerSettings().WriteSlopeVolt)},
          Match{"getSlopeVolt",                 GET_FUNC(GetControllerSettings().GetSlopeVolt())},
          Match{"getSlopeVoltMin",              GET_FORMAT(GetControllerSettings().GetSlopeVoltMin())},
          Match{"getSlopeVoltMax",              GET_FORMAT(GetControllerSettings().GetSlopeVoltMax())},
          Match{"getSlopeStartupVoltSp",        GET_FORMAT(GetControllerSettings().GetSlopeStartupVoltSp())},
          Match{"getSlopeVoltSp",               GET_FORMAT(GetControllerSettings().GetSlopeVoltSp())},

          // Slopes Current
          Match{"setSlopeCurrentMs",            SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeCurrentMs)},
          Match{"setSlopeStartupCurrentMs",     SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeStartupCurrentMs)},

          Match{"setSlopeCurrentRaw",           SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeCurrentRaw)},
          Match{"setSlopeStartupCurrentRaw",    SET_FUNC_DOUBLE(GetControllerSettings().SetSlopeStartupCurrentRaw)},

          Match{"cmdSlopeCurrentWrite",         CMD_API(GetControllerSettings().WriteSlopeCurrent)},
          Match{"getSlopeCurrent",              GET_FUNC(GetControllerSettings().GetSlopeCurrent())},
          Match{"getSlopeCurrentMin",           GET_FORMAT(GetControllerSettings().GetSlopeCurrentMin())},
          Match{"getSlopeCurrentMax",           GET_FORMAT(GetControllerSettings().GetSlopeCurrentMax())},
          Match{"getSlopeStartupCurrentSp",     GET_FORMAT(GetControllerSettings().GetSlopeStartupCurrentSp())},
          Match{"getSlopeCurrentSp",            GET_FORMAT(GetControllerSettings().GetSlopeCurrentSp())},
          // -------------------------------------------------------------------------------
          // clang-format on
      }) {}

#undef CMD_API
#undef GET_FORMAT
#undef GET_FUNC
#undef GET_MEMBER
#undef SET_FUNC_DOUBLE
#undef SET_FUNC_UINT

std::string Handler::handle(const std::string &message) {
    try {
        m_RegatronComm->autoConnect();
        for (const auto &m : m_Matchers) {
            if (auto response = m.handle(message)) {
                return response.value();
            }
        }

        // Default not found message
        LOG_WARN(R"(No match for message "{}")", message);
        return NACK;

    } catch (const CommException &e) {
        LOG_CRITICAL(
            R"(CommException: Regatron communication exception "{}" when handling message "{}". Device TCIO will be closed.)",
            e.what(), message);

        // Update the communication status for logging
        m_RegatronComm->ReadCommStatus();

        LOG_CRITICAL(R"(Device TCIO will be closed, comm status "{}")", m_RegatronComm->getCommStatus());

        // Reset communication and DLL
        m_RegatronComm->disconnect();

    } catch (const std::invalid_argument &e) {
        LOG_CRITICAL(
            R"(Invalid Argument: Exception "{}" When handling message "{}")",
            e.what(), message);

    } catch (const std::runtime_error &e) {
        LOG_CRITICAL(
            R"(Runtime Error: Unexpected runtime error "{}" when handling message "{}")",
            e.what(), message);
    }
    return NACK;
}
} // namespace Regatron
