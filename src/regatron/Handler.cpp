
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <array>
#include <string>

namespace Regatron {
Handler::Handler(std::shared_ptr<Regatron::Comm> regatronComm)
    : m_Regatron(std::move(regatronComm)),
      m_Matchers({
          // Simple readings
          Match{"Voltage",
                [this]() {
                    auto readings = this->m_Regatron->getReadings();
                    return fmt::format("{}",
                                       readings->m_ModActualOutVoltageMon);
                },
                [this](double param) {
                    auto readings = this->m_Regatron->getReadings();
                    readings->m_ModActualOutVoltageMon += param;
                    return ACK;
                }},
          // T_ErrorTree32
          Match{"SysWarnTree",
                [this]() {
                    auto readings = this->m_Regatron->getReadings();
                    return readings->toString(readings->m_SysWarningTree32Mon);
                },
                nullptr},
          Match{"ModWarnTree",
                [this]() {
                    auto readings = this->m_Regatron->getReadings();
                    return readings->toString(readings->m_ModWarningTree32Mon);
                },
                nullptr},
          Match{"SysErrTree",
                [this]() {
                    auto readings = this->m_Regatron->getReadings();
                    return readings->toString(readings->m_SysErrorTree32Mon);
                },
                nullptr},
          Match{"ModErrTree",
                [this]() {
                    auto readings = this->m_Regatron->getReadings();
                    return readings->toString(readings->m_ModErrorTree32Mon);
                },
                nullptr}
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
