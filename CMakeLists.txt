cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name, my project isn't very descriptive
project(REGATRON_INTERFACE CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#   1. Release: high optimization level, no debug info, code or asserts.
#   2. Debug: No optimization, asserts enabled, [custom debug (output) code enabled],
#   debug info included in executable (so you can step through the code with a
#   debugger and have address to source-file:line-number translation).
#   3. RelWithDebInfo: optimized, *with* debug info, but no debug (output) code or asserts.
#   4. MinSizeRel: same as Release but optimizing for size rather than speed.
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

option(ENABLE_CONAN "Use Conan for dependency management" ON)
if(ENABLE_CONAN)
  include(cmake/Conan.cmake)
  run_conan()
endif()

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests.")
  add_subdirectory(tests)
endif()

include(vendor/CMakeLists.txt)

add_subdirectory(src)
