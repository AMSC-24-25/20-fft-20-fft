cmake_minimum_required(VERSION 3.22)
project(fft_project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C++20 and OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")
find_package(OpenMP REQUIRED)

# Add CUDA support
option(HAS_CUDA "Enable CUDA support" ON)
if (HAS_CUDA)
    message(STATUS "CUDA support requested by the user")
    include(CheckLanguage)
    check_language(CUDA)
    if(CMAKE_CUDA_COMPILER)
        enable_language(CUDA)
        message(STATUS "CUDA found: enabling CUDA FFT")
        set(HAS_CUDA TRUE)
        add_definitions(-DHAS_CUDA)
    else()
        message(WARNING "CUDA not found: compiling without CUDA FFT")
        set(HAS_CUDA FALSE)
    endif()
else ()
    message(STATUS "CUDA support deactivated by the user")
endif ()

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
