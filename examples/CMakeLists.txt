###################################
# Fast Fourier Transform Examples #
###################################
# Basic 1D FFT example with matplot++
add_executable(
        example-fourier_transform_solver
        fourier_transform_solver.cpp
)

# 1D FFT example to compare performance of different FFT libraries
add_executable(
        example-fourier_transform_solver_performance
        fourier_transform_solver_performance.cpp
)

# Application of 2D FFT to image processing
add_executable(
        example-fourier_transform_solver_image
        fourier_transform_solver_image.cpp
)

if (HAS_CUDA)
    message(STATUS "CUDA found: the examples will be running on GPU")
    # Basic 1D FFT example with matplot++
    target_link_libraries(example-fourier_transform_solver
            PUBLIC matplot
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
    # 1D FFT example to compare performance of different FFT libraries
    target_link_libraries(example-fourier_transform_solver_performance
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
    # Application of 2D FFT to image processing
    target_link_libraries(example-fourier_transform_solver_image
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
    # Application of 3D FFT to video processing
    add_executable(
            example-fourier_transform_solver_video
            fourier_transform_solver_video.cpp
    )
    target_link_libraries(
            example-fourier_transform_solver_video
            PRIVATE ${OpenCV_LIBS}
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
    # Application of 3D FFT to video processing with RGB channels
    add_executable(
            example-fourier_transform_solver_video_rgb
            fourier_transform_solver_video_rgb.cpp
    )
    target_link_libraries(
            example-fourier_transform_solver_video_rgb
            PRIVATE ${OpenCV_LIBS}
            PRIVATE signal_processing
            PRIVATE signal_processing_cuda
    )
else ()
    target_link_libraries(example-fourier_transform_solver
            PUBLIC matplot
            PRIVATE signal_processing
    )
    target_link_libraries(example-fourier_transform_solver_image
            PRIVATE signal_processing
    )
    target_link_libraries(example-fourier_transform_solver_performance
            PRIVATE signal_processing
    )
endif ()


###################################
# Haar Wavelet Transform Examples #
###################################
# Basic 1D Haar Wavelet Transform example
add_executable(
        example-haar_wavelet_transform_solver_1d
        haar_wavelet_transform_1d.cpp
)
target_link_libraries(
        example-haar_wavelet_transform_solver_1d
        PRIVATE signal_processing
)
# Basic 2D Haar Wavelet Transform example
add_executable(
        example-haar_wavelet_transform_solver_2d
        haar_wavelet_transform_2d.cpp
)
target_link_libraries(
        example-haar_wavelet_transform_solver_2d
        PRIVATE signal_processing
)
# Harr Wavelet Transform
add_executable(
        example-haar_wavelet_transform_solver
        haar_wavelet_transform.cpp
)
target_link_libraries(
        example-haar_wavelet_transform_solver
        PRIVATE signal_processing
)