cmake_minimum_required(VERSION 3.22)
project(fft_project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C++20 and OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")
find_package(OpenMP REQUIRED)

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
endif ()

# Dependencies and src
include_directories(external src)

# Add library to the build
add_subdirectory(src)

# Optionally add examples
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
    # Detect if gnuplot is installed (required by matplot++)
    find_program(GNUPLOT_EXECUTABLE gnuplot)
    if (NOT GNUPLOT_EXECUTABLE)
        message(FATAL_ERROR "Gnuplot not found. Please install it with 'sudo apt-get install gnuplot'.")
    endif()
    # Detect if matplot++ is installed
    if(EXISTS "${CMAKE_SOURCE_DIR}/external/matplotplusplus")
        add_subdirectory(external/matplotplusplus)
    else()
        message(FATAL_ERROR
                "The directory 'external/matplotplusplus' does not exist. " +
                "Did you forget to clone the repository? " +
                "Go to the root of the repository and run the following command: " +
                "git submodule update --init --recursive"
        )
    endif()
    # Detect if OpenCV is installed
    find_package(OpenCV REQUIRED)
    if (NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV not found. Please install it with 'sudo apt-get install libopencv-dev'.")
    endif()
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    # Add examples
    add_subdirectory(examples)
endif()
