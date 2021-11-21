file(GLOB REGATRON_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(regatron ${REGATRON_SRC_FILES})
target_link_libraries(
    regatron
    PRIVATE project_options
            project_warnings
            log
            net
            ${REGATRON_LIBRARIES}
            asio::asio
            spdlog::spdlog
            fmt::fmt)
target_include_directories(regatron PRIVATE "${PROJECT_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(
    regatron SYSTEM PRIVATE ${REGATRON_INCLUDE})
set_target_properties(
    regatron
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")