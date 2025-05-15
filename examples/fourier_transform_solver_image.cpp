/**
 * @file fourier_transform_solver_image.cpp
 * @brief This file demonstrates the correctness of the FFT and IFFT algorithms (2D)
 * by applying them to an image.
 */

#include <iostream>
#include <cmath>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "config-loader/json-configuration-loader.hpp"
#include "fourier-transform-solver/fast-fourier-transform/fast-fourier-transform.hpp"
#include "fourier-transform-solver/inverse-fast-fourier-transform/inverse-fast-fourier-transform.hpp"
#include "utils/timestamp.hpp"



int main() {
    /**
     * Ask the user to choose between the dog image and the eiffel-tower image.
     */
    printf("Do you want to use the dog image or the eiffel-tower image? (d/e) ");
    char choice;
    std::cin >> choice;
    if (choice != 'd' && choice != 'e') {
        std::cerr << "Invalid choice. Please enter 'd' for dog image or 'e' for eiffel-tower image." << std::endl;
        return 1;
    }
    printf("You chose %s image\n", choice == 'd' ? "dog" : "eiffel-tower");

    /**
     * Load the image using stb_image.h
     */
    int width, height, channels;
    const std::string filepath_in = choice == 'd' ? "examples/resources/dog.png" : "examples/resources/eiffel-tower.png";
    std::ostringstream filepath_out_oss;
    filepath_out_oss << "examples/output/fft-" << choice << createReadableTimestamp("_%Y%m%d_%H%M%S") << ".png";
    const std::string filepath_out = filepath_out_oss.str();
    // load the image
    unsigned char* image = stbi_load(
        filepath_in.c_str(),
        &width, &height, &channels, 3
    );
    // check if the image was loaded successfully
    if (image == nullptr) {
        std::cerr << "Error loading image" << std::endl;
        return 1;
    }
    printf("Image loaded successfully: (h: %d, w: %d), %d channel(s)\n", height, width, channels);

    /**
     * Convert the image to a vector of complex numbers.
     */
    std::vector<std::complex<double>> R, G, B;
    R.reserve(width * height);
    G.reserve(width * height);
    B.reserve(width * height);

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            const int idx = (y * width + x) * 3;
            R.emplace_back(image[idx + 0], 0.0);
            G.emplace_back(image[idx + 1], 0.0);
            B.emplace_back(image[idx + 2], 0.0);
        }
    printf("Image converted to complex numbers\n");


    /**
     * Core: Compute the FFT and IFFT of the image.
     */
    printf("Starting FFT computation...\n");

    const auto start_time = std::chrono::high_resolution_clock::now();
    fft::solver::FastFourierTransform<2> solver(std::array{static_cast<size_t>(height), static_cast<size_t>(width)});
    fft::solver::InverseFastFourierTransform<2> inverse_solver(std::array{static_cast<size_t>(height), static_cast<size_t>(width)});
    solver.compute(R, fft::solver::ComputationMode::OPENMP);
    printf("R FFT computed\n");
    solver.compute(G, fft::solver::ComputationMode::OPENMP);
    printf("G FFT computed\n");
    solver.compute(B, fft::solver::ComputationMode::OPENMP);
    printf("B FFT computed\n");
    inverse_solver.compute(R, fft::solver::ComputationMode::OPENMP);
    printf("R IFFT computed\n");
    inverse_solver.compute(G, fft::solver::ComputationMode::OPENMP);
    printf("G IFFT computed\n");
    inverse_solver.compute(B, fft::solver::ComputationMode::OPENMP);
    printf("B IFFT computed\n");
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    printf("FFT computation completed. Time taken %ld ms\n", duration);

    /**
     * Save the FFT result to a file.
     */
    std::vector<unsigned char> output(width * height * 3);
    for (size_t i = 0; i < R.size(); ++i) {
        output[i * 3 + 0] = static_cast<unsigned char>(std::clamp(std::real(R[i]), 0.0, 255.0));
        output[i * 3 + 1] = static_cast<unsigned char>(std::clamp(std::real(G[i]), 0.0, 255.0));
        output[i * 3 + 2] = static_cast<unsigned char>(std::clamp(std::real(B[i]), 0.0, 255.0));
    }
    printf("Image converted back to unsigned char\n");
    // check if the image was saved successfully
    if (
        stbi_write_png(
            filepath_out.c_str(),
            width, height, 3, output.data(), width * 3
        ) == 0
    ) {
        std::cerr << "Error saving image, make sure the path is correct or you have write permissions" << std::endl;
        return 1;
    }
    printf("Image saved successfully in %s\n", filepath_out.c_str());

    return 0;
}
