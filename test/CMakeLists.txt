# automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(catch_main PRIVATE project_options)

  
add_executable(
    instrumentator_tests
    instrumentator_tests.cpp)

target_link_libraries(
    instrumentator_tests PRIVATE
        project_warnings
        project_options
        catch_main)

target_include_directories(
    instrumentator_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${REGATRON_INTERFACE_SOURCE_DIR}/src")

catch_discover_tests(
  instrumentator_tests
  TEST_PREFIX
  "instrumentator."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=constexpr.xml)

# Disable the constexpr portion of the test, and build again this allows us to
# have an executable that we can debug when things go wrong with the constexpr
# testing
add_executable(relaxed_instrumentator_tests instrumentator_tests.cpp)
target_link_libraries(
  relaxed_instrumentator_tests PRIVATE project_options project_warnings
                                  catch_main)
target_compile_definitions(
  relaxed_instrumentator_tests PRIVATE
                                  -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)


catch_discover_tests(
  relaxed_instrumentator_tests
  TEST_PREFIX
  "relaxed_instrumentator."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=relaxed_instrumentator.xml)

# ---------- Regatron
add_executable(linuxTcioSample linuxTcioSample.cpp)

target_include_directories( linuxTcioSample PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_LIST_DIR}/../vendor/Regatron/V3.80.00 30072014 (Linux)/include")


target_link_libraries(
      linuxTcioSample PRIVATE project_options project_warnings
      "${CMAKE_CURRENT_LIST_DIR}/../vendor/Regatron/V3.80.00 30072014 (Linux)/Linux/x64/libtcio.so"
     )
