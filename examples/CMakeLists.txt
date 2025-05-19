##########
# OpenCV #
##########
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install it with 'sudo apt-get install libopencv-dev'.")
endif()
include_directories( ${OpenCV_INCLUDE_DIRS} )


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


target_link_libraries(example-fourier_transform_solver
        PRIVATE matplot
        PRIVATE signal_processing
)
target_link_libraries(example-fourier_transform_solver_image
        PRIVATE signal_processing
)
target_link_libraries(example-fourier_transform_solver_performance
        PRIVATE signal_processing
)


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


###################################
# JPEG Image Compression Examples #
###################################
# Basic JPEG image compression example
add_executable(
        example-jpeg_image_compression
        jpeg_image_compression.cpp
)
target_link_libraries(
        example-jpeg_image_compression
        PRIVATE signal_processing
)