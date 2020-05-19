add_executable(linuxTcioSample linuxTcioSample.cpp)

target_include_directories(linuxTcioSample PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(linuxTcioSample SYSTEM
                           PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../vendor/Regatron/V3.80.00 30072014 (Linux)/include")

target_link_libraries(
    linuxTcioSample
    PRIVATE project_options project_warnings
            "${CMAKE_CURRENT_LIST_DIR}/../vendor/Regatron/V3.80.00 30072014 (Linux)/Linux/x64/libtcio.so")
