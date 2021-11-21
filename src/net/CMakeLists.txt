file(GLOB NET_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(net ${NET_SRC_FILES})
target_link_libraries(
    net
    PRIVATE project_options
            project_warnings
            log
            asio::asio
            spdlog::spdlog
            fmt::fmt)

target_include_directories(net PRIVATE "${PROJECT_INCLUDE_DIR}")
set_target_properties(
    net
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")