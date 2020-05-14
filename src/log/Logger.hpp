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
/*
#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)
#define LOG_DEBUG_PREFIX                                                       \
    __FILE__ " (function {})"                                                  \
             " line " TOSTRING(__LINE__) ": "

#define LOG_DEBUG_TRACE(FORMAT, ...)                                           \
    ::Utils::Logger::getLogger()->trace(LOG_DEBUG_PREFIX FORMAT,               \
                                        __PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG_INFO(FORMAT, ...)                                            \
    ::Utils::Logger::getLogger()->info(LOG_DEBUG_PREFIX, __PRETTY_FUNCTION__,  \
                                       __VA_ARGS__)
#define LOG_DEBUG_WARN(FORMAT, ...)                                            \
    ::Utils::Logger::getLogger()->warn(LOG_DEBUG_PREFIX, __PRETTY_FUNCTION__,  \
                                       __VA_ARGS__)
#define LOG_DEBUG_ERROR(FORMAT, ...)                                           \
    ::Utils::Logger::getLogger()->error(LOG_DEBUG_PREFIX, __PRETTY_FUNCTION__, \
                                        __VA_ARGS__)
#define LOG_DEBUG_CRITICAL(FORMAT, ...)                                        \
    ::Utils::Logger::getLogger()->critical(LOG_DEBUG_PREFIX,                   \
                                           __PRETTY_FUNCTION__, __VA_ARGS__)
*/
#define LOG_TRACE(...)    ::Utils::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     ::Utils::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::Utils::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::Utils::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Utils::Logger::getLogger()->critical(__VA_ARGS__)
