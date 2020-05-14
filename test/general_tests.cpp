#include "catch2/catch.hpp"

#include <chrono>
#include <thread>

#include "log/Logger.hpp"
#include "utils/Instrumentator.hpp"

TEST_CASE(R"(Testing "log")", "[log]") {
    LOG_TRACE("lorem ipsum dolor sit amet");
    LOG_INFO("lorem ipsum dolor sit amet");
    LOG_WARN("lorem ipsum dolor sit amet");
    LOG_ERROR("lorem ipsum dolor sit amet");
    LOG_CRITICAL("lorem ipsum dolor sit amet");
}

constexpr auto DELAY_1 = std::chrono::milliseconds(80);
constexpr auto DELAY_2 = std::chrono::milliseconds(50);

void doWork1() {
    INSTRUMENTATOR_PROFILE_FUNCTION();
    LOG_INFO("Doing work 1 ...");
    std::this_thread::sleep_for(DELAY_1);
}
void doWork2() {
    INSTRUMENTATOR_PROFILE_FUNCTION();
    LOG_INFO("Doing work 2 ...");
    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_2));
}

TEST_CASE("Testing instrumentator", "[instrumentator]") {
    INSTRUMENTATOR_PROFILE_BEGIN_SESSION("Instrumentator test", "test.json");
    doWork1();
    doWork2();
    INSTRUMENTATOR_PROFILE_END_SESSION();
}
