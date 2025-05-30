add_executable(
        benchmark-fourier_transform
        fourier_transform.cpp
        utils.hpp
)
target_link_libraries(
        benchmark-fourier_transform
        PRIVATE benchmark::benchmark
        PRIVATE signal_processing
)
