#pragma once

#include "spdlog/spdlog.h"
#include <memory>

namespace Utils {
class Logger {
  private:
    static std::shared_ptr<spdlog::logger> defaultLogger;

  public:
    static void                                    Init();
    inline static std::shared_ptr<spdlog::logger> &getLogger() {
        return defaultLogger;
    }
};
} // namespace Utils

#define LOG_TRACE(...)    ::Utils::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     ::Utils::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::Utils::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::Utils::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Utils::Logger::getLogger()->critical(__VA_ARGS__)