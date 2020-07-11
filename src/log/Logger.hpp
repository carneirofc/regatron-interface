#pragma once

#include "spdlog/spdlog.h"
#include <memory>

/**
 * Always call Utils::Logger::Init()
 */
namespace Utils {
class Logger {
  private:
    static std::shared_ptr<spdlog::logger> defaultLogger;

  public:
    static void Init(spdlog::level::level_enum level = spdlog::level::trace);
    inline static std::shared_ptr<spdlog::logger> &getLogger() {
        if (!defaultLogger) {
            Init();
        }
        return defaultLogger;
    }
};
} // namespace Utils

// clang-format: off
template <typename... ARGS> constexpr auto LOG_TRACE(const ARGS &... args) {
    return ::Utils::Logger::getLogger()->trace(args...);
}
template <typename... ARGS> constexpr auto LOG_DEBUG(const ARGS &... args) {
    return ::Utils::Logger::getLogger()->debug(args...);
}
template <typename... ARGS> constexpr auto LOG_INFO(const ARGS &... args) {
    return ::Utils::Logger::getLogger()->info(args...);
}
template <typename... ARGS> constexpr auto LOG_WARN(const ARGS &... args) {
    return ::Utils::Logger::getLogger()->warn(args...);
}
template <typename... ARGS> constexpr auto LOG_ERROR(const ARGS &... args) {
    return ::Utils::Logger::getLogger()->error(args...);
}
template <typename... ARGS> constexpr auto LOG_CRITICAL(const ARGS &... args) {
    return ::Utils::Logger::getLogger()->critical(args...);
}
// clang-format: on