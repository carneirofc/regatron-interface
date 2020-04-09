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
    static constexpr const char *GET = "get";
    static constexpr const char *SET = "set";
    static constexpr const char *CMD = "cmd";

    const std::string                        m_BasePattern;
    const std::string                        m_SetFormat;
    const std::string                        m_GetPattern;
    const std::string                        m_SetPattern;
    const std::function<std::string()>       m_GetHandleFunc;
    const std::function<std::string(double)> m_SetHandleFunc;

    CommandType getCommandType(const std::string &message) {
        if (message.starts_with(GET)) {
            return CommandType::getCommand;
        }
        if (message.starts_with(CMD)) {
            return CommandType::cmdCommand;
        }
        if (message.starts_with(SET)) {
            return CommandType::setCommand;
        }
        return CommandType::invalidCommand;
    }

  public:
    /** @note: get only constructor */
    Match(const std::string basePattern, std::function<std::string()> getHandle)
        : Match(basePattern, "%lf", getHandle, nullptr) {}

    //  /** @note: Default setFormat "%lf" for double */
    //  Match(const std::string basePattern, std::function<std::string()>
    //  getHandle,
    //        std::function<std::string(double)> setHandle)
    //      : Match(basePattern, "%lf", getHandle, setHandle) {}

    Match(const std::string basePattern, const std::string setFormat,
          std::function<std::string()>       getHandle,
          std::function<std::string(double)> setHandle)
        : m_BasePattern(basePattern), m_SetFormat(setFormat),
          m_GetPattern(fmt::format("{}{}\n", GET, m_BasePattern)),
          m_SetPattern(
              fmt::format("{}{} {}\n", SET, m_BasePattern, m_SetFormat)),
          m_GetHandleFunc(std::move(getHandle)),
          m_SetHandleFunc(std::move(setHandle)) {}

    auto toString() const {
        return fmt::format("[Match](base_pattern \"{}\". set format \"{}\")",
                           m_BasePattern, m_SetFormat);
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

        } else if (m_SetHandleFunc != nullptr && message.starts_with(SET) &&
                   (param = handleSet(message.c_str()))) {
            commandType = CommandType::setCommand;
        } else {
            commandType = CommandType::invalidCommand;
        }

        switch (commandType) {
        default:
            return {};

        case CommandType::cmdCommand:
            return {fmt::format("{}{} {}\n", CMD, m_BasePattern,
                                m_GetHandleFunc())};

        case CommandType::getCommand:
            return {fmt::format("{}{} {}\n", GET, m_BasePattern,
                                m_GetHandleFunc())};

        case CommandType::setCommand:
            return {fmt::format("{}{} {}\n", SET, m_BasePattern,
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
