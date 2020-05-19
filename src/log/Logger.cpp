#include "Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Utils {
std::shared_ptr<spdlog::logger> Logger::defaultLogger;

void Utils::Logger::Init(spdlog::level::level_enum level) {
    defaultLogger = spdlog::stdout_color_mt("main");
    defaultLogger->set_level(level);
}
} // namespace Utils
