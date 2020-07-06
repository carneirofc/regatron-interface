cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name, my project isn't very descriptive
project(REGATRON_INTERFACE CXX)
include(cmake/StandardProjectSettings.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's
    # change
    target_precompile_headers(
        project_options
        INTERFACE
        <vector>
        <string>
        <map>
        <utility>)
endif()

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "asio/1.16.1@")
set(CONAN_EXTRA_OPTIONS "")

include(cmake/Conan.cmake)
run_conan()
if(MSVC)
    SET(REGATRON_LIBRARIES
        #"${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014/Windows/x64/Release/tcio.lib"
        #"${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014/Windows/x64/Release_Stdcall/tcio_stdcall.lib"
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.88.00 28042020/Windows/x86_x64/Release/tcio.lib"
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.88.00 28042020/Windows/x86_x64/Release_Stdcall/tcio_stdcall.lib"

    )
    SET(REGATRON_TCIO_DLL
        #"${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014/Windows/x64/Release/tcio.dll"
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.88.00 28042020/Windows/x86_x64/Release/tcio.dll"        
    )
    SET(REGATRON_TCIO_STDCALL_DLL
        #"${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014/Windows/x64/Release_Stdcall/tcio_stdcall.dll"
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.88.00 28042020/Windows/x86_x64/Release_Stdcall/tcio_stdcall.dll"    )
    
    SET(REGATRON_INCLUDE
        #"${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014/include"
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.88.00 28042020/include"
    )
else()
    SET(REGATRON_LIBRARIES
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014 (Linux)/Linux/x64/libtcio.so"
    )
    SET(REGATRON_INCLUDE
        "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014 (Linux)/include"
    )
endif()

if(ENABLE_TESTING)
    enable_testing()
    message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")
    add_subdirectory(test)
    add_subdirectory(test_tcio)
endif()

add_subdirectory(src)
