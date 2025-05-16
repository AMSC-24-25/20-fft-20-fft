#include <iostream>
#include <fstream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "compression/jpeg_image_compression/image/image.hpp"
#include "compression/jpeg_image_compression/compressed_image/compressed_image.hpp"
#include "transforms/discrete_cosine_transform/base_discrete_cosine_transform.hpp"
#include "transforms/discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.hpp"

namespace signal_processing::compression::jpeg_image_compression
{
    // #################### CONSTRUCTORS ####################

    Image::Image(std::vector<std::vector<double>> inputMatrix): img_matrix(inputMatrix) {}

    Image::Image(const char* image_path){
        this->img_matrix = load_from_png(image_path);
    }

    // #################### PUBLIC ####################

    const void Image::save_as_png(const std::string path){
        if(this->img_matrix.empty()){
            throw std::invalid_argument("Error: image matrix is empty, cannot save to png.");
        }

        const size_t rows = this->img_matrix.size();
        const size_t cols = this->img_matrix[0].size();

        // Convert the image into grayscale (pixel values between 0,255) and store it as 1D vector
        std::vector<unsigned char> image_data(rows * cols);

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                // clamp value between 0,255
                image_data[r * cols + c] = std::min(255.0, std::max(0.0, this->img_matrix[r][c]));
            }
        }
        // Save the image using stb_image_write
        if (stbi_write_png(path.c_str(), cols, rows, 1, image_data.data(), cols) == 0) {
            std::cerr << "Error: Could not save grayscale image" << std::endl;
            throw std::runtime_error("Error: Could not save grayscale image");
        }

        std::cout << "Image written successfully in a png file!" << std::endl;
    }

    CompressedImage Image::compress(){
        if (this->img_matrix.empty()) {
            throw std::invalid_argument("Error: image matrix is empty, cannot be compressed.");
        }

        const size_t rows = this->img_matrix.size();
        const size_t cols = this->img_matrix[0].size();
        std::vector<std::vector<double>> compressed = std::vector(rows, std::vector<double>(cols));

        // Create the constant matrix Q (quantization matrix) --> is a 8x8
        std::vector<std::vector<double>> Q = {
            { 16, 11, 10, 16, 24, 40, 51, 61},
            { 12, 12, 14, 19, 26, 58, 60, 55},
            { 14, 13, 16, 24, 40, 57, 69, 56},
            { 14, 17, 22, 29, 51, 87, 80, 62},
            { 18, 22, 37, 56, 68, 109, 103, 7},
            { 24, 35, 55, 64, 81, 104, 113, 92},
            { 49, 64, 78, 87, 103, 121, 120, 101},
            { 72, 92, 95, 98, 112, 100, 103, 99}
        };

        // Split up the image into blocks of 8 × 8 pixels
        int submatrixSize = 8;
        // sanity check if the image_data size are multiple of submatrixSize
        if(rows % submatrixSize != 0 || cols % submatrixSize != 0) {
            std::cerr << "Error: the image is not compressible since its sizes are not multiple of "
                      << submatrixSize << std::endl;
            throw std::runtime_error("Error: the image is not compressible since its sizes are not multiple of 8");
        }

        #pragma omp parallel for
        for (int r = 0; r < rows; r += submatrixSize) {
            for (int c = 0; c < cols; c += submatrixSize) {
                jpeg_compression(r, c, submatrixSize, compressed, Q);
            }
        }

        CompressedImage compressedImage = CompressedImage(compressed);
        return compressedImage;
    }

    // #################### PRIVATE ####################

    void Image::jpeg_compression(
        const int r,
        const int c,
        const int submatrixSize,
        std::vector<std::vector<double>>& compressed,
        std::vector<std::vector<double>>& Q
    ) {
        // 1. Create a copy of the submatrix
        std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
        for (int i=0; i<submatrixSize; ++i){
            for (int j=0; j<submatrixSize; ++j){
                // 2. Subtract 128 from each entry, so that the entries are now integers between -128 and 127.
                submatrix[i][j] = this->img_matrix[r+i][c+j] - 128.0;
            }
        }

        // 3. Take the 2-dimensional discrete cosine transform (dct) of the block.
        dct::solver::DiscreteCosineTransform dct_solver = dct::solver::DiscreteCosineTransform();
        const dct::solver::ComputationMode cm = dct::solver::ComputationMode::OPENMP;
        dct_solver.compute(submatrix, cm);

        // 4. Divide the block elementwise by a constant matrix Q known as a quantization matrix,
        //    and then round each entry to the nearest integer
        for (int i=0; i<submatrixSize; ++i){
            for (int j=0; j<submatrixSize; ++j){
                // 5. Save the result in the big matrix of the image
                compressed[r+i][c+j] = std::round(submatrix[i][j] / Q[i][j]);
            }
        }
    }

    const std::vector<std::vector<double>> Image::load_from_png(const char* image_path){
        // Load the image using stb_image
        int width, height, channels;
        unsigned char* image_data = stbi_load(image_path, &width, &height, &channels, 1);

        if (!image_data) {
            std::cerr << "Error: Could not load image " << image_path << std::endl;
            throw std::runtime_error("Error: could not load image from the .png file");
        }

        std::cout << "Image loaded: " << width << "x" << height << " with " << channels << " channels." << std::endl;

        // Create the vector of vector of double that represent the image data
        std::vector<std::vector<double>> image_matrix(height, std::vector<double>(width));

        for (int r = 0; r < height; ++r){
            for (int c = 0; c < width; ++c){
                int index = (r*width + c) * 1; // *1 => 1 channel (Greyscale), *3 => channels (RGB)
                image_matrix[r][c] = image_data[index]; // (- 128) --> point 2 of jpc compression
            }
        }

        stbi_image_free(image_data); // Free the image data after loading it into the matrix

        return image_matrix;
    }
}
