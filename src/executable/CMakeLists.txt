file(GLOB EXECUTABLE_NET_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(regatron_interface ${EXECUTABLE_NET_SRC_FILES})
target_include_directories(regatron_interface PRIVATE "${PROJECT_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(regatron_interface SYSTEM PRIVATE ${REGATRON_INCLUDE})
target_link_libraries(
    regatron_interface
    PRIVATE project_options
            project_warnings
            log
            net
            regatron
            asio::asio
            docopt::docopt
            spdlog::spdlog
            fmt::fmt)
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

