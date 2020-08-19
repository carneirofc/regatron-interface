#include "Logger.hpp"

#include <filesystem>

#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace Utils {
std::shared_ptr<spdlog::logger> Logger::defaultLogger;
constexpr int                   LOG_FILE_COUNT = 2;
constexpr int                   LOG_FILE_SIZE = 1024 * 1024 * 8;

void Utils::Logger::Init(spdlog::level::level_enum level, const char* logFileName) {
    spdlog::init_thread_pool(8192, 1);
    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFileName, LOG_FILE_SIZE, LOG_FILE_COUNT);
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    std::vector<spdlog::sink_ptr> sinks{fileSink, consoleSink};
    defaultLogger = std::make_shared<
        spdlog::async_logger>("main", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    defaultLogger->set_level(level);

    spdlog::register_logger(defaultLogger);
    auto path = std::filesystem::current_path();
    path.append(logFileName);
    defaultLogger->debug("---- Log Init ----");
    defaultLogger->debug(R"(file: "{}")", path.string());
    defaultLogger->debug("count: {}", LOG_FILE_COUNT);
    defaultLogger->debug("size: {:.3} Mb", static_cast<float>(LOG_FILE_SIZE)/1024.0f/1024.0f);
}
} // namespace Utils
