#pragma once

#include "net/Handler.hpp"
#include "regatron/Comm.hpp"
#include <chrono>
#include <iostream>
#include <optional>

namespace Regatron {
const std::string NACK{"NACK\n"};
const std::string ACK{"ACK\n"};

enum class CommandType : int {
    unsupportedMessage = -10,
    invalidCommand     = -1,
    getCommand,
    setCommand,
    cmdCommand
};

/**
 * @todo: impplement an elegant dispatcher?
 * @todo: Migrate to a template class?
 * ideas: Have an object checker list that will forward the message to the
 * correct handler method.
 * */
class Match {
  private:
    static constexpr const char *GET = "get"; // Not parameterized
    static constexpr const char *SET = "set"; // Parameterized

    const std::string &                      m_CommandString;
    const std::string                        m_SetFormat;
    const std::string                        m_GetPattern;
    const std::string                        m_SetPattern;
    const std::function<std::string()>       m_GetHandleFunc;
    const std::function<std::string(double)> m_SetHandleFunc;

    CommandType getCommandType(const std::string &message) {
        if (message.starts_with(GET)) {
            return CommandType::getCommand;
        }
        if (message.starts_with(SET)) {
            return CommandType::setCommand;
        }
        return CommandType::invalidCommand;
    }

    Match(const std::string &commandString, const std::string setFormat,
          std::function<std::string()>       getHandle,
          std::function<std::string(double)> setHandle)
        : m_CommandString(commandString), m_SetFormat(setFormat),
          m_GetPattern(fmt::format("{}\n", m_CommandString)),
          m_SetPattern(fmt::format("{} {}\n", m_CommandString, m_SetFormat)),
          m_GetHandleFunc(std::move(getHandle)),
          m_SetHandleFunc(std::move(setHandle)) {}

  public:
    /** @note: get only constructor */
    Match(const std::string &          commandString,
          std::function<std::string()> getHandle)
        : Match(commandString, "%lf", std::move(getHandle), nullptr) {}

    /** @note: set only constructor */
    Match(const std::string &                commandString,
          std::function<std::string(double)> setHandle)
        : Match(commandString, "%lf", nullptr, std::move(setHandle)) {}

    auto toString() const {
        return fmt::format(R"([Match](command" {} ". set format " {} "))",
                           m_CommandString, m_SetFormat);
    }

    /** throws: May throw something (std::invalid_argument)! */
    std::optional<double> handleSet(const char *message) const {
        double data{0};
        int    r = std::sscanf(message, m_SetPattern.c_str(), &data);
        return r == 1 ? std::optional<double>{data} : std::nullopt;
    }

    /** Respond a message according to the command type.
     * @return: A response to the client. following the pattern
     * '{}{} {}\n' COMMAND (get/set), BasePattern, handle return
     * @throws:
     * */
    std::optional<std::string> handle(const std::string &message) const {
        CommandType commandType;

        std::optional<double> param;

        if (m_GetHandleFunc != nullptr && message == m_GetPattern) {
            commandType = CommandType::getCommand;

        } else if (m_SetHandleFunc != nullptr &&
                   (param = handleSet(message.c_str()))) {
            commandType = CommandType::setCommand;
        } else {
            commandType = CommandType::invalidCommand;
        }

        switch (commandType) {
        default:
            return {};

        case CommandType::getCommand:
            return {fmt::format("{} {}\n", m_CommandString, m_GetHandleFunc())};

        case CommandType::setCommand:
            return {fmt::format("{} {}\n", m_CommandString,
                                m_SetHandleFunc(param.value()))};
        }
    }
};

class Handler : public Net::Handler {
  public:
    Handler(std::shared_ptr<Regatron::Comm> regatronComm);

  private:
    std::shared_ptr<Regatron::Comm> m_Regatron;
    std::vector<Match>              m_Matchers;
    const std::string               handle(const std::string &message) override;
};
} // namespace Regatron
