add_executable(
        benchmark-fourier_transform
        fourier_transform.cpp
        utils.hpp
)

if (HAS_CUDA)
    target_link_libraries(
            benchmark-fourier_transform
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
else ()
    message(WARNING "CUDA not found: CUDA benchmarks will not be available.")
    target_link_libraries(
            benchmark-fourier_transform
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
    )
endif ()
