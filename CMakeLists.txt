cmake_minimum_required(VERSION 3.22)
project(20_fft_20_fft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add C++20 and OpenMP flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")

# Detect if the OpenMP library is available
find_package(OpenMP REQUIRED)

# Detect if gnuplot is installed
find_program(GNUPLOT_EXECUTABLE gnuplot)
if (NOT GNUPLOT_EXECUTABLE)
    message(FATAL_ERROR "Gnuplot not found. Please install it with 'sudo apt-get install gnuplot'.")
endif()

# External libraries
include_directories(external)
add_subdirectory(external/matplotplusplus)
# Header files
include_directories(include)

# If you are a 'Politecnico di Milano' student, you can use the following lines to include the MK library
# Otherwise, you can remove them...
# The MOX department provides a MK library:
# "Environment modules for scientific computing libraries and packages,
#  with portable x86-64 Linux binaries.
#  Developed at MOX (Politecnico di Milano) and based on Lmod."
# https://github.com/pcafrica/mk
include_directories(
        # /u/sw/toolchains/gcc-glibc/11.2.0/base/include
        # To include the Eigen library:
        # /u/sw/toolchains/gcc-glibc/11.2.0/pkgs/eigen/3.3.9/include/eigen3
        # To include the LIS library:
        # /u/sw/toolchains/gcc-glibc/11.2.0/pkgs/lis/2.0.30/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/config-loader/json-configuration-loader.cpp
    src/signal-generator/time-domain-signal-generator.cpp
    src/signal-generator/space-domain-signal-generator.cpp
    src/config-loader/json-field-handler.cpp
    src/signal-saver/csv-signal-saver.cpp
    src/utils/timestamp.cpp
    src/utils/bit-reversal.cpp
    src/fourier-transform-solver/parallel-1d-fast-ft.cpp
    src/fourier-transform-solver/parallel-1d-inverse-fast-ft.cpp
    src/fourier-transform-solver/sequential-1d-fast-ft.cpp
    src/fourier-transform-solver/sequential-1d-inverse-fast-ft.cpp
)

# Header files
set(HEADERS
    include/config-loader/abstract-configuration-loader.hpp
    include/config-loader/json-configuration-loader.hpp
    include/signal-generator/abstract-signal-generator.hpp
    include/signal-generator/time-domain-signal-generator.hpp
    include/signal-generator/space-domain-signal-generator.hpp
    include/config-loader/json-field-handler.hpp
    include/signal-saver/abstract-file-signal-saver.hpp
    include/signal-saver/csv-signal-saver.hpp
    include/utils/timestamp.hpp
    include/fourier-transform-solver/abstract-fourier-transform-solver.hpp
    include/fourier-transform-solver/sequential-1d-fast-ft.hpp
    include/fourier-transform-solver/parallel-1d-fast-ft.hpp
    include/fourier-transform-solver/parallel-1d-inverse-fast-ft.hpp
    include/utils/bit-reversal.hpp
    include/fourier-transform-solver/sequential-1d-inverse-fast-ft.hpp
)

# Add the default executable
add_executable(main ${SOURCES} ${HEADERS})
# Link the matplot library
target_link_libraries(main PUBLIC matplot)

##################
# Parallel build #
##################
# Since matplot can be slow to compile, we suggest to use the parallel_build target
# Naive benchmark: 1 core -> 1m 40s, 12 cores -> 30s (Intel i7-8700K, approx.)
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
    message(WARNING
            "Your system has ${N} CPU cores. You can use the following command to build the project faster: 'make -f ./Makefile -C . parallel_build'."
    )
endif()
# Add the parallel_build target
add_custom_target(parallel_build
        COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)