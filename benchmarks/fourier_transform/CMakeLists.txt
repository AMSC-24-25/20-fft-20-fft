add_executable(
        benchmark-fourier_transform_sequential
        fourier_transform_sequential.cpp
        utils.hpp
)
add_executable(
        benchmark-fourier_transform_openmp
        fourier_transform_openmp.cpp
        utils.hpp
)

if (HAS_CUDA)
    add_executable(
            benchmark-fourier_transform_cuda
            fourier_transform_cuda.cpp
            utils.hpp
    )
    target_link_libraries(
            benchmark-fourier_transform_cuda
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
    target_link_libraries(
            benchmark-fourier_transform_sequential
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
    target_link_libraries(
            benchmark-fourier_transform_openmp
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
else ()
    message(WARNING "CUDA not found: CUDA benchmarks will not be built")
    target_link_libraries(
            benchmark-fourier_transform_sequential
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
    )
    target_link_libraries(
            benchmark-fourier_transform_openmp
            PRIVATE benchmark::benchmark
            PRIVATE signal_processing
    )
endif ()
