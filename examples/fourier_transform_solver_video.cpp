#include <complex>
#include <iostream>
#include <vector>

#include "opencv4/opencv2/videoio.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/core/mat.hpp"
#include "opencv4/opencv2/core/types.hpp"
#include "opencv4/opencv2/core/hal/interface.h"

#include "fourier_transform/base_fourier_transform.hpp"
#include "fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp"
#include "fourier_transform/inverse_fast_fourier_transform/inverse_fast_fourier_transform.hpp"

int main() {
    /**
    * 1. Load the Video using OpenCV
     */
    cv::VideoCapture cap("examples/resources/cats.mp4");
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
    for (int i = 0; i < 128; ++i)
    {
        cap.read(frame);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);  // Optional: use grayscale for simplicity

        // crop to 1024*1024
        int x = (frame.cols - 1024) / 2;
        int y = (frame.rows - 1024) / 2;
        cv::Rect roi(x, y, 1024, 1024);
        frame = frame(roi);

        frame.convertTo(frame, CV_64F);  // Ensure double precision for FFT
        frames.push_back(frame);
    }
    int depth = frames.size(); // Number of frames
    int height = frames[0].rows;
    int width  = frames[0].cols;
    printf("Extracted %d frames of size %dx%d.\n", depth, height, width);


    /**
     * 3. Create a 3D Array and Apply FFT
     */
    std::vector<std::vector<std::vector<std::complex<double>>>> volume(
        depth, std::vector<std::vector<std::complex<double>>>(
            height, std::vector<std::complex<double>>(width)
        )
    );
    // Fill volume from frames
    for (int z = 0; z < depth; ++z)
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                volume[z][y][x] = std::complex<double>(frames[z].at<double>(y, x), 0.0);
    printf("Volume created with dimensions %dx%dx%d.\n", depth, height, width);


    /**
     * 4. Apply Cooley-Tukey 3D FFT
     */
    std::vector<std::complex<double>> volume_data(depth * height * width);

    for (int z = 0; z < depth; ++z)
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int idx = z * height * width + y * width + x;
                volume_data[idx] = std::complex<double>(frames[z].at<double>(y, x), 0.0);
            }
    printf("Volume data flattened for FFT.\n");


    auto start_time = std::chrono::high_resolution_clock::now();
    fft::solver::FastFourierTransform<3> solver({static_cast<size_t>(depth), static_cast<size_t>(height), static_cast<size_t>(width)});
    fft::solver::InverseFastFourierTransform<3> i_solver({static_cast<size_t>(depth), static_cast<size_t>(height), static_cast<size_t>(width)});
    solver.compute(volume_data, fft::solver::ComputationMode::OPENMP);
    i_solver.compute(volume_data, fft::solver::ComputationMode::OPENMP);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    printf("FFT and IFFT applied.\n");
    printf("FFT computation time: %.2f seconds\n", elapsed.count());

    std::vector<cv::Mat> reconstructed_frames;
    for (int z = 0; z < depth; ++z) {
        cv::Mat frame(height, width, CV_64F);
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                int idx = z * height * width + y * width + x;
                frame.at<double>(y, x) = volume_data[idx].real();  // Discard imaginary
            }
        frame.convertTo(frame, CV_8U);
        reconstructed_frames.push_back(frame);
    }
    printf("Reconstructed frames from FFT.\n");

    cv::VideoWriter writer("examples/output/output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 60, cv::Size(width, height), false);
    for (const auto& f : reconstructed_frames)
        writer.write(f);
    writer.release();

    printf("Reconstructed video saved as output.avi.\n");
}