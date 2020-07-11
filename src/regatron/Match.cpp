#include "regatron/Match.hpp"

namespace Regatron {

Match::Match(std::string &&commandString, std::string &&setFormat,
             std::function<std::string()> &&      getHandle,
             std::function<std::string(double)> &&setHandle)
    : m_CommandString(commandString), m_SetFormat(setFormat),
      m_GetPattern(fmt::format("{}\n", m_CommandString)),
      m_SetPattern(fmt::format("{} {}\n", m_CommandString, m_SetFormat)),
      m_GetHandleFunc(getHandle),
      m_SetHandleFunc(setHandle) {
    LOG_TRACE(toString());
}

/** @note: get only constructor */
Match::Match(std::string &&                 commandString,
             std::function<std::string()> &&getHandle)
    : Match(std::move(commandString), "%lf", std::move(getHandle), nullptr) {}

/** @note: set only constructor */
Match::Match(std::string &&                       commandString,
             std::function<std::string(double)> &&setHandle)
    : Match(std::move(commandString), "%lf", nullptr, std::move(setHandle)) {}

std::string Match::toString() const {
    return fmt::format(R"([Match](m_CommandString"{}"))", m_CommandString);
}

CommandType Match::getCommandType(const std::string &message) {
    if (message.starts_with(GET)) {
        return CommandType::getCommand;
    }
    if (message.starts_with(SET)) {
        return CommandType::setCommand;
    }
    return CommandType::invalidCommand;
}

std::optional<double> Match::handleSet(const char *message) const {
    double data{0};
    int    r = std::sscanf(message, m_SetPattern.c_str(), &data);
    return r == 1 ? std::optional<double>{data} : std::nullopt;
}
/*
template <class Container>
void Match::split(const std::string &str, Container &cont) {
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(cont));
}
*/
std::optional<std::string> Match::handle(const std::string &message) const {
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

    if (commandType == CommandType::invalidCommand) {
        return {};
    }

    // LOG_DEBUG(R"(command: "{}" message: "{}")", m_CommandString,  message);
    INSTRUMENTATOR_PROFILE_SCOPE(m_CommandString.c_str());

    switch (commandType) {
    default:
        LOG_CRITICAL(
            R"(Logic error ! Unknown commandType at "Match::handle".)");
        return {};

    case CommandType::getCommand: {
        return {fmt::format("{} {}\n", m_CommandString, m_GetHandleFunc())};
    }
    case CommandType::setCommand: {
        return {fmt::format("{} {}\n", m_CommandString,
                            m_SetHandleFunc(param.value()))};
    }
    }
}

} // namespace Regatron
