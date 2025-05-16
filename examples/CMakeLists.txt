###################################
# Fast Fourier Transform Examples #
###################################
# Basic 1D FFT example with matplot++
add_executable(
        example-fourier_transform_solver
        ${FFT-LIBRARY}
        fourier_transform_solver.cpp
)
target_link_libraries(example-fourier_transform_solver
        PUBLIC matplot
        PRIVATE signal_processing
)

# 1D FFT example to compare performance of different FFT libraries
add_executable(
        example-fourier_transform_solver_performance
        ${FFT-LIBRARY}
        fourier_transform_solver_performance.cpp
)
target_link_libraries(example-fourier_transform_solver_performance
        PRIVATE signal_processing
)

# Application of 2D FFT to image processing
add_executable(
        example-fourier_transform_solver_image
        ${FFT-LIBRARY}
        fourier_transform_solver_image.cpp
)
target_link_libraries(example-fourier_transform_solver_image
        PRIVATE signal_processing
)

# Application of 3D FFT to video processing
add_executable(
        example-fourier_transform_solver_video
        ${FFT-LIBRARY}
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
        ${FFT-LIBRARY}
        fourier_transform_solver_video_rgb.cpp
)
target_link_libraries(
        example-fourier_transform_solver_video_rgb
        PRIVATE ${OpenCV_LIBS}
        PRIVATE signal_processing
        PRIVATE signal_processing_cuda
)
