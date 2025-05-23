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
include_directories(
        external
        src
)
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

set (
        FFT-LIBRARY

        # config-loader
        src/config-loader/abstract-configuration-loader.hpp
        src/config-loader/json-configuration-loader.cpp
        src/config-loader/json-configuration-loader.hpp
        src/config-loader/json-field-handler.cpp
        src/config-loader/json-field-handler.hpp

        # fourier-transform-solver
        src/fourier-transform-solver/fourier-transform-solver.hpp
        # - algorithms
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft.hpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft.cpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft-openmp.hpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft-openmp.cpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft.hpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft.cpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft-openmp.hpp
        src/fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft-openmp.cpp
        # - fast-fourier-transform
        src/fourier-transform-solver/fast-fourier-transform/fast-fourier-transform.hpp
        # - inverse-fast-fourier-transform
        src/fourier-transform-solver/inverse-fast-fourier-transform/inverse-fast-fourier-transform.hpp

        # signal-generator
        src/signal-generator/abstract-signal-generator.hpp
        src/signal-generator/space-domain-signal-generator.cpp
        src/signal-generator/space-domain-signal-generator.hpp
        src/signal-generator/time-domain-signal-generator.cpp
        src/signal-generator/time-domain-signal-generator.hpp

        # signal-saver
        src/signal-saver/abstract-file-signal-saver.hpp
        src/signal-saver/csv-signal-saver.cpp
        src/signal-saver/csv-signal-saver.hpp

        # utils
        src/utils/timestamp.cpp
        src/utils/timestamp.hpp
        src/utils/bit-reversal.cpp
        src/utils/bit-reversal.hpp

        #discrete-cosine-transform-solver
        src/discrete-cosine-transform-solver/discrete-cosine-transform-solver.hpp
        src/discrete-cosine-transform-solver/discrete-cosine-transform-solver.cpp
        src/discrete-cosine-transform-solver/discrete-cosine-transform/discrete-cosine-transform.hpp
        src/discrete-cosine-transform-solver/discrete-cosine-transform/discrete-cosine-transform.cpp
        src/discrete-cosine-transform-solver/inverse-discrete-cosine-transform/inverse-discrete-cosine-transform.hpp
        src/discrete-cosine-transform-solver/inverse-discrete-cosine-transform/inverse-discrete-cosine-transform.cpp
        src/discrete-cosine-transform-solver/algorithms/dct.hpp
        src/discrete-cosine-transform-solver/algorithms/dct.cpp
        src/discrete-cosine-transform-solver/algorithms/dct_openMP.hpp
        src/discrete-cosine-transform-solver/algorithms/dct_openMP.cpp
        src/discrete-cosine-transform-solver/algorithms/idct.hpp
        src/discrete-cosine-transform-solver/algorithms/idct.cpp
        src/discrete-cosine-transform-solver/algorithms/idct_openMP.hpp
        src/discrete-cosine-transform-solver/algorithms/idct_openMP.cpp

        # zigzag-scan
        src/zigzag_scan/zigzag_scan.hpp
        src/zigzag_scan/zigzag_scan.cpp

        # rle-compressor
        src/rle_compressor/rle_compressor.hpp
        src/rle_compressor/rle_compressor.cpp

        # jpeg-image-compression
        src/jpeg_image_compression/image/image.hpp
        src/jpeg_image_compression/image/image.cpp
        src/jpeg_image_compression/compressed_image/compressed_image.hpp
        src/jpeg_image_compression/compressed_image/compressed_image.cpp
)

# Add the default executable
add_executable(main ${FFT-LIBRARY} src/main.cpp)
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