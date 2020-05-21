#pragma once
#include "log/Logger.hpp"
#include "utils/Instrumentator.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>

namespace Regatron {

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

    const std::string                        m_CommandString;
    const std::string                        m_SetFormat;
    const std::string                        m_GetPattern;
    const std::string                        m_SetPattern;
    const std::function<std::string()>       m_GetHandleFunc;
    const std::function<std::string(double)> m_SetHandleFunc;

    CommandType getCommandType(const std::string &message);
    Match(const std::string commandString, const std::string setFormat,
          std::function<std::string()>       getHandle,
          std::function<std::string(double)> setHandle);

    template <class Container>
    void split(const std::string &str, Container &cont);

  public:
    /** @note: get only constructor */
    Match(const std::string            commandString,
          std::function<std::string()> getHandle);

    /** @note: set only constructor */
    Match(const std::string                  commandString,
          std::function<std::string(double)> setHandle);

    std::string toString() const;

    /** throws: May throw something (std::invalid_argument)! */
    std::optional<double> handleSet(const char *message) const;

    /**
     * Respond a message according to the command type.
     *
     * @return: A response to the client. following the pattern
     *     '{} {}\n' Pattern, handle method response.
     * @throws:
     * */
    std::optional<std::string> handle(const std::string &message) const;
};
} // namespace Regatron
