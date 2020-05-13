#include "catch2/catch.hpp"

#include <chrono>
#include <thread>

#include "log/Logger.hpp"
#include "utils/Instrumentator.hpp"

    void
    doWork2() {
    INSTRUMENTATOR_PROFILE_FUNCTION();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
void doWork1() {
    INSTRUMENTATOR_PROFILE_FUNCTION();
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
}
TEST_CASE("Testing instrumentator", "[instrumentator]") {
    INSTRUMENTATOR_PROFILE_BEGIN_SESSION("Instrumentator test", "test.json");
    INSTRUMENTATOR_PROFILE_END_SESSION();
}
