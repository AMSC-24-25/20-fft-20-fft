/**
 * @file fourier_transform_solver_video_rgb.cpp
 * @brief This program demonstrates how to apply 3D FFT and IFFT
 *        on a video using OpenCV and the Fast Fourier Transform (FFT) library.
 *
 * @details The program extracts frames from a video, applies FFT and IFFT
 *          on the RGB channels separately, and reconstructs the video.
 */

#include <complex>
#include <filesystem>
#include <iostream>
#include <vector>

#include "opencv4/opencv2/videoio.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/core/mat.hpp"
#include "opencv4/opencv2/core/types.hpp"
#include "opencv4/opencv2/core/hal/interface.h"

#include "transforms/fourier_transform/base_fourier_transform.hpp"
#include "transforms/fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp"
#include "transforms/fourier_transform/inverse_fast_fourier_transform/inverse_fast_fourier_transform.hpp"
#include "utils/timestamp.hpp"

int main() {
    /**
    * 1. Load the Video using OpenCV
     */
    const std::string input_file = "examples/resources/cats-resize.mp4";
    std::ostringstream filepath_out_oss;
    filepath_out_oss << "examples/output/fft-"
                     << signal_processing::utils::timestamp::createReadableTimestamp("%Y%m%d_%H%M%S")
                     << ".avi";
    // check if the output folder exists
    if (!std::filesystem::exists("examples/output")) {
        std::cerr << "Output folder does not exist. Creating it..." << std::endl;
        std::filesystem::create_directory("examples/output");
    }
    cv::VideoCapture cap(input_file);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file." << std::endl;
        return -1;
    }
    printf("Video loaded successfully.\n");

    /**
     * 2. Extract and Store Frames
     */
    cv::Mat frame;
    std::vector<cv::Mat> frames_r, frames_g, frames_b;
    for (int i = 0; i < 256; ++i)
    {
        if (!cap.read(frame)) break;

        // crop to 1024*1024
        // int x = (frame.cols - 1024) / 2;
        // int y = (frame.rows - 1024) / 2;
        // cv::Rect roi(x, y, 1024, 1024);
        // frame = frame(roi);

        std::vector<cv::Mat> channels(3);
        cv::split(frame, channels);  // BGR

        for (int c = 0; c < 3; ++c)
            channels[c].convertTo(channels[c], CV_64F);

        frames_b.push_back(channels[0]);
        frames_g.push_back(channels[1]);
        frames_r.push_back(channels[2]);
    }
    int depth = frames_b.size(); // Number of frames
    int height = frames_b[0].rows;
    int width  = frames_b[0].cols;
    printf("Extracted %d frames of size %dx%d.\n", depth, height, width);


    /**
     * 3. Create a 3D Array and Apply FFT
     */
    std::vector<std::complex<double>> volume_r(depth * height * width);
    std::vector<std::complex<double>> volume_g(depth * height * width);
    std::vector<std::complex<double>> volume_b(depth * height * width);

    # pragma omp parallel for
    for (int z = 0; z < depth; ++z)
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int idx = z * height * width + y * width + x;
                volume_r[idx] = std::complex<double>(frames_r[z].at<double>(y, x), 0.0);
                volume_g[idx] = std::complex<double>(frames_g[z].at<double>(y, x), 0.0);
                volume_b[idx] = std::complex<double>(frames_b[z].at<double>(y, x), 0.0);
            }
    printf("Volume data flattened for FFT.\n");

    /**
     * 4. Apply Cooley-Tukey 3D FFT
     */
    signal_processing::fft::solver::FastFourierTransform<3> solver(
        {static_cast<size_t>(depth), static_cast<size_t>(height), static_cast<size_t>(width)}
    );
    signal_processing::fft::solver::InverseFastFourierTransform<3> i_solver(
        {static_cast<size_t>(depth), static_cast<size_t>(height), static_cast<size_t>(width)}
    );

    auto start_time = std::chrono::high_resolution_clock::now();
    solver.compute(volume_r, signal_processing::fft::solver::ComputationMode::CUDA);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    printf("R FFT: %f seconds\n", elapsed.count());


    start_time = std::chrono::high_resolution_clock::now();
    i_solver.compute(volume_r, signal_processing::fft::solver::ComputationMode::CUDA);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed = end_time - start_time;
    printf("R IFFT: %f seconds\n", elapsed.count());


    start_time = std::chrono::high_resolution_clock::now();
    solver.compute(volume_g, signal_processing::fft::solver::ComputationMode::CUDA);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed = end_time - start_time;
    printf("G FFT: %f seconds\n", elapsed.count());


    start_time = std::chrono::high_resolution_clock::now();
    i_solver.compute(volume_g, signal_processing::fft::solver::ComputationMode::CUDA);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed = end_time - start_time;
    printf("G IFFT: %f seconds\n", elapsed.count());


    start_time = std::chrono::high_resolution_clock::now();
    solver.compute(volume_b, signal_processing::fft::solver::ComputationMode::CUDA);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed = end_time - start_time;
    printf("B FFT: %f seconds\n", elapsed.count());


    start_time = std::chrono::high_resolution_clock::now();
    i_solver.compute(volume_b, signal_processing::fft::solver::ComputationMode::CUDA);
    end_time = std::chrono::high_resolution_clock::now();
    elapsed = end_time - start_time;
    printf("B IFFT: %f seconds\n", elapsed.count());


    /**
     * 5. Reconstruct Frames
     */
    std::vector<cv::Mat> reconstructed_frames;
    for (int z = 0; z < depth; ++z) {
        cv::Mat r(height, width, CV_64F);
        cv::Mat g(height, width, CV_64F);
        cv::Mat b(height, width, CV_64F);

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int idx = z * height * width + y * width + x;
                r.at<double>(y, x) = volume_r[idx].real();
                g.at<double>(y, x) = volume_g[idx].real();
                b.at<double>(y, x) = volume_b[idx].real();
            }

        cv::Mat r8u, g8u, b8u;
        r.convertTo(r8u, CV_8U);
        g.convertTo(g8u, CV_8U);
        b.convertTo(b8u, CV_8U);

        std::vector<cv::Mat> channels = {b8u, g8u, r8u};  // OpenCV = BGR
        cv::Mat color;
        cv::merge(channels, color);

        reconstructed_frames.push_back(color);
    }
    printf("Reconstructed frames from FFT.\n");

    /**
     * 6. Save the Reconstructed Video
     */
    cv::VideoWriter writer(
        filepath_out_oss.str(),
        cv::VideoWriter::fourcc('M','J','P','G'),
        60, cv::Size(width, height), true
    );
    for (const auto& f : reconstructed_frames)
        writer.write(f);
    writer.release();
    printf("Reconstructed video saved to %s\n", filepath_out_oss.str().c_str());
}
