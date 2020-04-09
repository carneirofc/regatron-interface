
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <array>
#include <string>

namespace Regatron {
#define GET_READING(member)                                                    \
    [this]() {                                                                 \
        auto readings = this->m_Regatron->getReadings();                       \
        return fmt::format("{}", readings->member);                            \
    }

#define GET_TREE_READING(member)                                               \
    [this]() {                                                                 \
        auto readings = this->m_Regatron->getReadings();                       \
        return readings->toString(readings->m_SysWarningTree32Mon);            \
    }

// @fixme: Do this in a way that does not require a macros.
Handler::Handler(std::shared_ptr<Regatron::Comm> regatronComm)
    : m_Regatron(std::move(regatronComm)),
      m_Matchers({
          // Commands with no response
          Match{"ModOutVoltage", GET_READING(m_ModActualOutVoltageMon)},
          // Simple readings
          Match{"ModOutVoltage", GET_READING(m_ModActualOutVoltageMon)},
          Match{"ModOutCurrent", GET_READING(m_ModActualOutCurrentMon)},
          Match{"ModOutPower", GET_READING(m_ModActualOutPowerMon)},
          Match{"ModRes", GET_READING(m_ModActualResMon)},
          Match{"ModState", GET_READING(m_ModState)},

          Match{"SysOutVoltage", GET_READING(m_SysActualOutVoltageMon)},
          Match{"SysOutCurrent", GET_READING(m_SysActualOutCurrentMon)},
          Match{"SysOutPower", GET_READING(m_SysActualOutPowerMon)},
          Match{"SysRes", GET_READING(m_SysActualResMon)},
          Match{"SysState", GET_READING(m_ModState)},

          // T_ErrorTree32
          Match{"ModWarnTree", GET_TREE_READING(m_ModWarningTree32Mon)},
          Match{"ModErrTree", GET_TREE_READING(m_ModErrorTree32Mon)},
          Match{"SysWarnTree", GET_TREE_READING(m_SysWarningTree32Mon)},
          Match{"SysErrTree", GET_TREE_READING(m_SysErrorTree32Mon)},
          //
      }) {}

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
