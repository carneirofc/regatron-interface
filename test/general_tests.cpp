#include "catch2/catch.hpp"

#include <chrono>
#include <thread>

#include "log/Logger.hpp"
#include "regatron/Readings.hpp"
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


TEST_CASE("Testing Slope calculation", "[slope]") {
    constexpr double TIME_TOL = 1e-7;

    const double CALC_MAX_TIME =
        Regatron::Slope::rawToTime(Regatron::Slope::MIN_RAW);
    const double CALC_MIN_TIME =
        Regatron::Slope::rawToTime(Regatron::Slope::MAX_RAW);

    const unsigned int CALC_MAX_RAW =
        Regatron::Slope::timeToRaw(Regatron::Slope::MIN_TIME);
    const unsigned int CALC_MIN_RAW =
        Regatron::Slope::timeToRaw(Regatron::Slope::MAX_TIME);

    REQUIRE(CALC_MAX_TIME == Regatron::Slope::MAX_TIME);
    REQUIRE(((CALC_MIN_TIME >= (Regatron::Slope::MIN_TIME - TIME_TOL)) &&
             (CALC_MIN_TIME <= (Regatron::Slope::MIN_TIME + TIME_TOL))));

    REQUIRE(CALC_MAX_RAW == Regatron::Slope::MAX_RAW);
    REQUIRE(CALC_MIN_RAW == Regatron::Slope::MIN_RAW);

    fmt::print("y = {} * x + {};  1 <= x <= 32000.\n", Regatron::Slope::SLOPE_A,
               Regatron::Slope::SLOPE_B);
    fmt::print("----------------------------------------\n");
    fmt::print("rawToTime x={} y={}s \n", Regatron::Slope::MIN_RAW,
               Regatron::Slope::rawToTime(Regatron::Slope::MIN_RAW));
    fmt::print("rawToTime x={} y={}s \n", Regatron::Slope::MAX_RAW,
               Regatron::Slope::rawToTime(Regatron::Slope::MAX_RAW));

    fmt::print("timeToRaw y={}s x={} \n", Regatron::Slope::MAX_TIME,
               Regatron::Slope::timeToRaw(Regatron::Slope::MAX_TIME));
    fmt::print("timeToRaw y={}s x={} \n", Regatron::Slope::MIN_TIME,
               Regatron::Slope::timeToRaw(Regatron::Slope::MIN_TIME));
}
