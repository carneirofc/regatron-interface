
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <array>
#include <string>

namespace Regatron {
#define GET_FUNC(func)                                                         \
    [this]() { return this->m_Regatron->getReadings()->func(); }

#define GET_MEMBER(member)                                                     \
    [this]() {                                                                 \
        auto readings = this->m_Regatron->getReadings();                       \
        return fmt::format("{}", readings->member);                            \
    }

#define GET_TREE_READING(member)                                               \
    [this]() {                                                                 \
        auto readings = this->m_Regatron->getReadings();                       \
        return readings->toString(readings->m_SysWarningTree32Mon);            \
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
          Match{"cmdReadSysErrTree", CMD_API(readSystemErrorTree32)},
          Match{"cmdReadModErrTree", CMD_API(readModuleErrorTree32)},
          Match{"cmdStoreParam", CMD_API(storeParameters)},
          Match{"cmdClearErrors", CMD_API(clearErrors)},
          Match{"cmdReadControlMode", CMD_API(readControlMode)},
          Match{"cmdReadControlInput", CMD_API(readRemoteControlInput)},

          // Simple readings
          Match{"getTemperatures", GET_FUNC(getTemperatures)},
          Match{"getModReadings", GET_FUNC(getModReadings)},
          Match{"getSysReadings", GET_FUNC(getSysReadings)},

          // Error + Warning T_ErrorTree32
          Match{"getModTree", GET_FUNC(getModTree)},
          Match{"getSysTree", GET_FUNC(getSysTree)},
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
