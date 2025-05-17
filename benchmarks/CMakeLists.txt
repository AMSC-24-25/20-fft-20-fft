if (NOT BUILD_BENCHMARKS)
    message(STATUS "Google Benchmark not found: benchmarks will not be built")
    return()
endif ()

find_package(benchmark REQUIRED)
if (NOT benchmark_FOUND)
    message(
            FATAL_ERROR
            "Google Benchmark not found. Please install it following the instructions at "
            "https://github.com/google/benchmark?tab=readme-ov-file#installation"
    )
endif ()

add_subdirectory(fourier_transform)
