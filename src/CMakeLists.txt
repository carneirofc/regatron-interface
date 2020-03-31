add_executable(main main.cpp
    log/Logger.cpp
    net/Handler.cpp
    net/Server.cpp
    regatron/Comm.cpp
    regatron/Handler.cpp
    regatron/IBC.cpp
    regatron/Readings.cpp
    regatron/Version.cpp)

target_include_directories(main PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CONAN_INCLUDE_DIRS}"
  "${CMAKE_CURRENT_LIST_DIR}/../../vendor/regatron/V3.80.00 30072014 (Linux)/include")


target_link_libraries(
  main PRIVATE project_options project_warnings
      "${CMAKE_CURRENT_LIST_DIR}/../../vendor/regatron/V3.80.00 30072014 (Linux)/Linux/x64/libtcio.so"
      CONAN_PKG::boost
      CONAN_PKG::catch2
      CONAN_PKG::docopt.cpp
      CONAN_PKG::spdlog
      CONAN_PKG::fmt)