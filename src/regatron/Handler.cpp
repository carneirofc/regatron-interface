
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <array>
#include <string>

namespace Regatron {
#define GET_FUNC(func)                                                         \
    [this]() { return this->m_Regatron->getReadings()->func; }

#define GET_MEMBER(member)                                                     \
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
          // Commands with no response
          Match{"cmdStoreParam", CMD_API(storeParameters)},
          Match{"cmdClearErrors", CMD_API(clearErrors)},

          // Simple readings
          Match{"getDSPVersion", GET_FUNC(getVersion()->m_DSPVersionString)},
          Match{"getDLLVersion", GET_FUNC(getVersion()->m_DLLVersionString)},
          Match{"getBootloaderVersion",
                GET_FUNC(getVersion()->m_MainDSPBootloaderVersionString)},
          Match{"getModulatorVersion",
                GET_FUNC(getVersion()->m_ModulatorDSPVersionString)},
          Match{"getPheripherieVersion",
                GET_FUNC(getVersion()->m_PeripherieDSPVersionString)},

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
