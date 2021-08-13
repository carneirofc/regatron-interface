add_library(log log/Logger.cpp)
target_link_libraries(
    log
    PRIVATE project_options
            project_warnings
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
target_include_directories(log PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")
set_target_properties(
    log
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")

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
set_target_properties(
    net
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")

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
set_target_properties(
    regatron
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")

add_executable(regatron_interface main.cpp)
target_include_directories(regatron_interface PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CONAN_INCLUDE_DIRS}")
target_include_directories(
    regatron_interface SYSTEM PRIVATE ${REGATRON_INCLUDE})
target_link_libraries(
    regatron_interface
    PRIVATE project_options
            project_warnings
            log
            net
            regatron
            CONAN_PKG::asio
            CONAN_PKG::docopt.cpp
            CONAN_PKG::spdlog
            CONAN_PKG::fmt)
set_target_properties(
    regatron_interface
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")

if (MSVC)
    add_custom_command(TARGET regatron_interface POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
           ${REGATRON_TCIO_DLL}
           ${REGATRON_TCIO_STDCALL_DLL}
           $<TARGET_FILE_DIR:regatron_interface>)
endif ()

