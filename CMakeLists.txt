### Setup cmake requirement ###
cmake_minimum_required(VERSION 3.10...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

### Setup project information ###
project(GPCWrapper 
    VERSION 1.0.0
    DESCRIPTION "C++ Wrapper for Generic Polygon Clipper"
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)

### Require out-of-source builds ###
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

### Compile source code ###
add_library(gpc STATIC "gpc.c" "gpc.h")
target_include_directories(gpc PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

### Compile wrapper code ###
add_library(gpcpp STATIC "gpcpp.cc" "gpcpp.h")
target_include_directories(gpcpp PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(gpcpp PRIVATE gpc)

### Compile test code ###
add_executable(test_gpc "test.cc")
target_include_directories(test_gpc PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(test_gpc PRIVATE gpcpp)

