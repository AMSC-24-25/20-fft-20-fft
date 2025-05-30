/**
 * @file fourier_transform_solver_video.cpp
 * @brief This program demonstrates how to apply 3D FFT and IFFT
 *        on a video using OpenCV and the Fast Fourier Transform (FFT) library.
 *
 * @details This example uses the Cooley-Tukey algorithm to perform FFT and IFFT
 *          on a 3D volume of frames extracted from a video.
 */

#include <complex>
#include <sys/stat.h>
#include <iostream>
#include <vector>

#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/core/hal/interface.h"

#include "signal_processing/signal_processing.hpp"

int main() {
    /**
    * 1. Load the Video using OpenCV
     */
    std::ostringstream filepath_out_oss;
    filepath_out_oss << "examples/output/fft-"
                     << sp::utils::timestamp::createReadableTimestamp("%Y%m%d_%H%M%S")
                     << ".avi";
    // check if the output folder exists
    struct stat info;
    if (stat("examples/output", &info) != 0 || !(info.st_mode & S_IFDIR)) {
        std::cerr << "Output folder does not exist. Creating it..." << std::endl;
        mkdir("examples/output", 0755);
    }
    cv::VideoCapture cap("examples/resources/cats-resize.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file." << std::endl;
        return -1;
    }
    printf("Video loaded successfully.\n");

    /**
     * 2. Extract and Store Frames
     */
    std::vector<cv::Mat> frames;
    cv::Mat frame;
    for (int i = 0; i < 256; ++i)
    {
        if (!cap.read(frame)) {
            std::cerr << "Error reading frame " << i << std::endl;
            break;
        }
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);  // Optional: use grayscale for simplicity

        // crop to 1024*1024
        // int x = (frame.cols - 1024) / 2;
        // int y = (frame.rows - 1024) / 2;
        // cv::Rect roi(x, y, 1024, 1024);
        // frame = frame(roi);

        frame.convertTo(frame, CV_64F);  // Ensure double precision for FFT
        frames.push_back(frame);
    }
    int depth = frames.size(); // Number of frames
    int height = frames[0].rows;
    int width  = frames[0].cols;
    printf("Extracted %d frames of size %dx%d.\n", depth, height, width);


    /**
     * 3. Create an array to give to FFT; Row-Major order!
     */
    std::vector<std::complex<double>> volume_data(depth * height * width);
    for (int z = 0; z < depth; ++z)
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int idx = z * height * width + y * width + x;
                volume_data[idx] = std::complex<double>(frames[z].at<double>(y, x), 0.0);
            }
    printf("Volume data flattened for FFT.\n");


    /**
     * 4. Apply Cooley-Tukey 3D FFT
     */
    auto start_time = std::chrono::high_resolution_clock::now();
    sp::fft::solver::FastFourierTransform<3> solver(
        {static_cast<size_t>(depth), static_cast<size_t>(height), static_cast<size_t>(width)}
    );
    sp::fft::solver::InverseFastFourierTransform<3> i_solver(
        {static_cast<size_t>(depth), static_cast<size_t>(height), static_cast<size_t>(width)}
    );
    solver.compute(volume_data, sp::fft::solver::ComputationMode::OPENMP);
    i_solver.compute(volume_data, sp::fft::solver::ComputationMode::OPENMP);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    printf("FFT and IFFT applied.\n");
    printf("FFT computation time: %.2f seconds\n", elapsed.count());

    /**
     * 5. Reconstruct the frames from the FFT output
     */
    std::vector<cv::Mat> reconstructed_frames;
    for (int z = 0; z < depth; ++z) {
        cv::Mat frame(height, width, CV_64F);
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int idx = z * height * width + y * width + x;
                frame.at<double>(y, x) = volume_data[idx].real();
            }
        frame.convertTo(frame, CV_8U);
        reconstructed_frames.push_back(frame);
    }
    printf("Reconstructed frames from FFT.\n");

    /**
     * 6. Save the reconstructed frames as a video
     */
    cv::VideoWriter writer(
        filepath_out_oss.str(),
        cv::VideoWriter::fourcc('M','J','P','G'),
        60,
        cv::Size(width, height),
        false
    );
    for (const auto& f : reconstructed_frames)
        writer.write(f);
    writer.release();
    printf("Reconstructed video saved as %s.\n", filepath_out_oss.str().c_str());
}
