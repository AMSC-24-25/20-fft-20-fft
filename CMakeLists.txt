cmake_minimum_required(VERSION 3.28)
project(20_fft_20_fft)

set(CMAKE_CXX_STANDARD 20)
# add C++20 and OpenMP flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")

include_directories(external)
include_directories(include)

# If you are a 'Politecnico di Milano' student, you can use the following lines to include the MK library
# Otherwise, you can remove them...
# The MOX department provides a MK library:
# "Environment modules for scientific computing libraries and packages,
#  with portable x86-64 Linux binaries.
#  Developed at MOX (Politecnico di Milano) and based on Lmod."
# https://github.com/pcafrica/mk
include_directories(
        /u/sw/toolchains/gcc-glibc/11.2.0/base/include
        # To include the Eigen library:
        # /u/sw/toolchains/gcc-glibc/11.2.0/pkgs/eigen/3.3.9/include/eigen3
        # To include the LIS library:
        # /u/sw/toolchains/gcc-glibc/11.2.0/pkgs/lis/2.0.30/include
)

add_executable(main src/main.cpp
        include/config-loader/abstract-configuration-loader.hpp
        include/config-loader/json-configuration-loader.hpp
        src/config-loader/json-configuration-loader.cpp
        include/signal-generator/abstract-signal-generator.hpp
        include/signal-generator/time-domain-signal-generator.hpp
        include/signal-generator/space-domain-signal-generator.hpp
        src/signal-generator/time-domain-signal-generator.cpp
        src/signal-generator/space-domain-signal-generator.cpp
)
