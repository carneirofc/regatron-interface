add_library(log log/Logger.cpp)
target_link_libraries(
    log
    PRIVATE project_options
            project_warnings
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
target_include_directories(log PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")

add_library(net net/Server.cpp)
target_link_libraries(
    net
    PRIVATE project_options
            project_warnings
            log
            # CONAN_PKG::asio
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)

target_include_directories(net PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")
target_include_directories(net SYSTEM PRIVATE "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/asio-asio-1-16-1/asio/include")

add_library(
    regatron
    regatron/Comm.cpp
    regatron/Handler.cpp
    regatron/IBC.cpp
    regatron/Match.cpp
    regatron/Readings.cpp
    regatron/Version.cpp)
target_link_libraries(
    regatron
    PRIVATE project_options
            project_warnings
            log
            net
            "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014 (Linux)/Linux/x64/libtcio.so"
            # CONAN_PKG::asio
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
target_include_directories(regatron PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")
target_include_directories(
    regatron SYSTEM PRIVATE "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/asio-asio-1-16-1/asio/include"
                            "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014 (Linux)/include")

add_executable(main main.cpp)
target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")
target_include_directories(
    main SYSTEM PRIVATE "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/asio-asio-1-16-1/asio/include"
                        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014 (Linux)/include")

target_link_libraries(
    main
    PRIVATE project_options
            project_warnings
            log
            net
            regatron
            CONAN_PKG::docopt.cpp
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
