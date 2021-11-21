file(GLOB LOG_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(log ${LOG_SRC_FILES})
target_link_libraries(
    log
    PRIVATE project_options
            project_warnings
            spdlog::spdlog
            fmt::fmt)
target_include_directories(log PRIVATE "${PROJECT_INCLUDE_DIR}")
set_target_properties(
    log
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")