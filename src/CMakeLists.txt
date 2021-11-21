find_package(asio)
find_package(docopt)
find_package(fmt)
find_package(spdlog)

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(log)
add_subdirectory(net)
add_subdirectory(regatron)
add_subdirectory(executable)