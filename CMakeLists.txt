cmake_minimum_required(VERSION 3.22)
project(signal_processing VERSION 1.0.0 LANGUAGES CXX)
set(SIGNAL_PROCESSING_VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C++20 and OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")
find_package(OpenMP REQUIRED)

# Dependencies and src
include_directories(src/external src)

# Add library to the build
add_subdirectory(src)

# Optionally add examples
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
    # Add examples
    add_subdirectory(examples)
endif()

# Optionally add benchmarks
option(BUILD_BENCHMARKS "Build benchmark programs" ON)
if(BUILD_BENCHMARKS)
    # Add benchmarks
    add_subdirectory(benchmarks)
endif ()
