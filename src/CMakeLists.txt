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
            CONAN_PKG::asio
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)

target_include_directories(net PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")

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
            ${REGATRON_LIBRARIES}
            CONAN_PKG::asio
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
target_include_directories(regatron PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")

target_include_directories(
    regatron SYSTEM PRIVATE ${REGATRON_INCLUDE})

add_executable(cons_regatron_interface main.cpp)
target_include_directories(cons_regatron_interface PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")
target_include_directories(
    cons_regatron_interface SYSTEM PRIVATE ${REGATRON_INCLUDE})

target_link_libraries(
    cons_regatron_interface
    PRIVATE project_options
            project_warnings
            log
            net
            regatron
            CONAN_PKG::docopt.cpp
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
