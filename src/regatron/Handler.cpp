
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <array>
#include <string>

namespace Regatron {
#define SET_FUNC_DOUBLE(func)                                                  \
    [this](double arg1) {                                                      \
        this->m_Regatron->getReadings()->func(arg1);                           \
        return ACK;                                                            \
    }

#define GET_FUNC(func)                                                         \
    [this]() { return this->m_Regatron->getReadings()->func; }

#define GET_FORMAT(member)                                                     \
    [this]() {                                                                 \
        auto readings = this->m_Regatron->getReadings();                       \
        return fmt::format("{}", readings->member);                            \
    }

#define CMD_API(member)                                                        \
    [this]() {                                                                 \
        this->m_Regatron->getReadings()->member();                             \
        return ACK;                                                            \
    }

// @fixme: Do this in a way that does not require a macros.
Handler::Handler(std::shared_ptr<Regatron::Comm> regatronComm)
    : m_Regatron(std::move(regatronComm)),
      m_Matchers({
          // clang-format off
          // Commands with no response
          Match{"cmdStoreParam", CMD_API(storeParameters)},
          Match{"cmdClearErrors", CMD_API(clearErrors)},

          // Simple readings
          Match{"getDSPVersion", GET_FUNC(getVersion()->m_DSPVersionString)},
          Match{"getDLLVersion", GET_FUNC(getVersion()->m_DLLVersionString)},
          Match{"getBootloaderVersion", GET_FUNC(getVersion()->m_MainDSPBootloaderVersionString)},
          Match{"getModulatorVersion", GET_FUNC(getVersion()->m_ModulatorDSPVersionString)},
          Match{"getPheripherieVersion", GET_FUNC(getVersion()->m_PeripherieDSPVersionString)},

          Match{"getDCLinkVoltage", GET_FUNC(getDCLinkVoltage())},
          Match{"getPrimaryCurrent", GET_FUNC(getPrimaryCurrent())},

          Match{"getTemperatures", GET_FUNC(getTemperatures())},
          Match{"getModReadings", GET_FUNC(getModReadings())},
          Match{"getSysReadings", GET_FUNC(getSysReadings())},
          Match{"getSysControlMode", GET_FUNC(getSysControlMode())},
          Match{"getModControlMode", GET_FUNC(getModControlMode())},
          Match{"getControlInput", GET_FUNC(getRemoteControlInput())},

          // Error + Warning T_ErrorTree32
          Match{"getModTree", GET_FUNC(getModTree())},
          Match{"getSysTree", GET_FUNC(getSysTree())},

          Match{"getModCurrentRef", GET_FORMAT(getModCurrentRef())},
          Match{"getModVoltageRef", GET_FORMAT(getModVoltageRef())},
          Match{"getModResistanceRef", GET_FORMAT(getModResistanceRef())},
          Match{"getModPowerRef", GET_FORMAT(getModPowerRef())},

          Match{"getSysCurrentRef", GET_FORMAT(getSysCurrentRef())},
          Match{"getSysVoltageRef", GET_FORMAT(getSysVoltageRef())},
          Match{"getSysResistanceRef", GET_FORMAT(getSysResistanceRef())},
          Match{"getSysPowerRef", GET_FORMAT(getSysPowerRef())},

          Match{"getSysCurrentRef", SET_FUNC_DOUBLE(setSysCurrentRef)},
          Match{"getSysVoltageRef", SET_FUNC_DOUBLE(setSysVoltageRef)},
          Match{"getSysResistanceRef", SET_FUNC_DOUBLE(setSysResistanceRef)},
          Match{"getSysPowerRef", SET_FUNC_DOUBLE(setSysPowerRef)},

          // clang-format on
      }) {}

#undef GET_FUNC
#undef GET_MEMBER
#undef CMD_API

const std::string Handler::handle(const std::string &message) {
    try {
        for (const auto &m : m_Matchers) {
            if (auto response = m.handle(message)) {
                return response.value();
            }
        }

        // Default not found message
        LOG_WARN("No match for message \"{}\"", message);
        return NACK;

        LOG_WARN("No compatible action for {}!", message);
    } catch (const std::invalid_argument &e) {
        LOG_CRITICAL("Exception: Invalid argument {} {}.", message, e.what());
    } catch (const std::runtime_error &e) {
        LOG_CRITICAL("Exception: A bug !{} {}.", message, e.what());
    }
    return NACK;
}
} // namespace Regatron
