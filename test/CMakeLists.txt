# automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
    include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
    include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(general_tests general_tests.cpp)
target_link_libraries(
    general_tests
    PRIVATE project_warnings
            project_options
            catch_main
            log
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)

target_include_directories(general_tests PRIVATE "${CONAN_INCLUDE_DIRS}" "${CMAKE_CURRENT_SOURCE_DIR}" "${REGATRON_INTERFACE_SOURCE_DIR}/src")

catch_discover_tests(
    general_tests
    TEST_PREFIX
    "general."
    EXTRA_ARGS
    -s
    --reporter=xml
    --out=constexpr.xml)

# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# things go wrong with the constexpr testing
add_executable(relaxed_general_tests general_tests.cpp)
target_link_libraries(
    relaxed_general_tests
    PRIVATE project_options
            project_warnings
            catch_main
            log
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
target_compile_definitions(relaxed_general_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
target_include_directories(relaxed_general_tests PRIVATE "${CONAN_INCLUDE_DIRS}"
                                                         "${CMAKE_CURRENT_SOURCE_DIR}"
                                                         "${REGATRON_INTERFACE_SOURCE_DIR}/src")

catch_discover_tests(
    relaxed_general_tests
    TEST_PREFIX
    "relaxed_general."
    EXTRA_ARGS
    -s
    --reporter=xml
    --out=relaxed_general.xml)

