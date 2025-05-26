##########
# OpenCV #
##########
find_package(OpenCV)
if (NOT OpenCV_FOUND)
    message(WARNING "OpenCV not found. Please install it with 'sudo apt-get install libopencv-dev'.")
else ()
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
endif ()

#############
# Matplot++ #
#############
# Detect if gnuplot is installed (required by matplot++)
find_program(GNUPLOT_EXECUTABLE gnuplot)
if (NOT GNUPLOT_EXECUTABLE)
    message(FATAL_ERROR "Gnuplot not found. Please install it with 'sudo apt-get install gnuplot'.")
endif()
# Detect if matplot++ is installed
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/examples/external/matplotplusplus")
    message(FATAL_ERROR "Matplot++ not found. Please clone it with 'git submodule update --init --recursive'.")
endif()
add_subdirectory(external/matplotplusplus)
message(STATUS "Matplot++ found: ${MATPLOT_VERSION}")


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
if (OpenCV_FOUND)
    # Application of 3D FFT to video processing
    add_executable(
            example-fourier_transform_solver_video
            fourier_transform_solver_video.cpp
    )
    # Application of 3D FFT to video processing (RGB)
    add_executable(
            example-fourier_transform_solver_video_rgb
            fourier_transform_solver_video_rgb.cpp
    )
    target_link_libraries(example-fourier_transform_solver_video
            PRIVATE ${OpenCV_LIBS}
            PRIVATE signal_processing
    )
    target_link_libraries(example-fourier_transform_solver_video_rgb
            PRIVATE ${OpenCV_LIBS}
            PRIVATE signal_processing
    )
else ()
    message(WARNING "OpenCV not found. Skipping video processing examples.")
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


###################################
# JPEG Image Compression Examples #
###################################
# Basic JPEG image compression example
add_executable(
        example-jpeg_compression
        jpeg_compression.cpp
)
target_link_libraries(
        example-jpeg_compression
        PRIVATE matplot
        PRIVATE signal_processing
)