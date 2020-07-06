add_executable(linuxTcioSample linuxTcioSample.cpp)

target_include_directories(linuxTcioSample PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(linuxTcioSample SYSTEM
                           PRIVATE ${REGATRON_INCLUDE})

target_link_libraries(
    linuxTcioSample
    PRIVATE project_options project_warnings
            ${REGATRON_LIBRARIES})
