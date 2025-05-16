#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>

#include "jpeg_image_compression/compressed_image/compressed_image.hpp"
#include "utils/zigzag_scan.hpp"
#include "utils/rle_compressor.hpp"
#include "discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.hpp"
#include "discrete_cosine_transform/inverse_discrete_cosine_transform/inverse_discrete_cosine_transform.hpp"
#include "jpeg_image_compression/image/image.hpp"

// #################### CONSTRUCTORS ####################

/**
 * Constructor that initializes the CompressedImage object from a given matrix by copying it.
 *
 * @param inputMatrix: 2D vector containing pixel values.
 */
CompressedImage::CompressedImage(std::vector<std::vector<double>> inputMatrix): compressed(inputMatrix) {}

/**
 * Constructor that loads the compressed image from a file .bin
 * @param compressed_image_path:  path to binary file;
 * @param option: 1 (raw binary), 2 (compressed binary).
 */
CompressedImage::CompressedImage(const std::string compressed_image_path, const int option) {
    if (option == 1){
        this->compressed = load_from_binary(compressed_image_path);
    }
    else if (option == 2){
        this->compressed = load_from_compressed_binary(compressed_image_path);
    }
    else {
        throw std::runtime_error(
            "Error: invalid option in ImageJPEG constructor. "
            "Acceptable ones are only option=1 for \"load compressed image from a binary file\", "
            "or option=2 for \"load compressed image form a compressed binary file (zigzag+rle)\""
        );
    }
}


// #################### PUBLIC ####################

/**
 * Function that saves compressed as a binary file.
 *
 * @param path: binary file path.
 */
const void CompressedImage::save_as_binary(const std::string& path) {
    if (this->compressed.empty()) {
        throw std::invalid_argument("Error: there is no compressed image to save as binary file.");
    }

    // Open the file in bynary mode to write
    std::ofstream file(path, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening binary file!" << std::endl;
        throw std::runtime_error("Error opening binary file!");
    }

    // Write matrix size at the beginning of the binary file (rows, cols)
    size_t rows = this->compressed.size();
    size_t cols = this->compressed[0].size();

    // reinterpret_cast<char*> --> to interpret each double as an array of byte
    file.write(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));

    // Write matrix's elements
    for (int r=0; r<rows; ++r) {
        for (int c=0; c<cols; ++c) {
            // since img_matrix[r][c] stays in a range of -128 e 127
            int8_t value = static_cast<int8_t>(this->compressed[r][c]);
            file.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
    }

    file.close();

    std::cout << "Compressed image written successfully in a binary file!" << std::endl;
}

/**
 * Function that saves compressed as a compressed binary file (with zigzag + RLE).
 *
 * @param path: compressed binary file path.
 */
const void CompressedImage::save_as_compressed_binary(const std::string& path){
    if (this->compressed.empty()) {
        throw std::invalid_argument("Error: there is no compressed image to save as binary file.");
    }

    int submatrixSize = 8;

    std::ofstream file(path, std::ios::binary);

    int rows = this->compressed.size();
    int cols = this->compressed[0].size();
    file.write(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.write(reinterpret_cast<char*>(&submatrixSize), sizeof(submatrixSize));

    for (int r = 0; r < rows; r += submatrixSize) {
        for (int c = 0; c < cols; c += submatrixSize) {
            // Create a copy of the submatrix and copy inside it the correspondin value of img_matrix
            std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
            for (int i = 0; i < submatrixSize; ++i)
                for (int j = 0; j < submatrixSize; ++j)
                    submatrix[i][j] = this->compressed[r+i][c+j];
            // Apply zigzag scan to the submatrix
            std::vector<double> zzVector = ZigZagScan::scan(submatrix);
            //save the submatrix in the binary file (using rle compression)
            save_compressed_submatrix(file, zzVector);
        }
    }

    file.close();

    std::cout << "Image matrix written successfully in a binary file using zigzag scan & rle compression!" << std::endl;
}

/**
 * Function that implements the jpeg decompression algorithm on compressed image matrix.
 *
 * @return: decompressed image.
 */
Image CompressedImage::decompress(){
    if(this->compressed.empty()){
        throw std::invalid_argument("Error: there is no compresed image to decompress.");
    }
    const size_t rows = this->compressed.size();
    const size_t cols = this->compressed[0].size();
    std::vector<std::vector<double>> decompressed = std::vector(rows, std::vector<double>(cols));

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
    const int submatrixSize = 8;
    // sanity check if the image_data size are multiple of submatrixSize
    if (rows % submatrixSize != 0 || cols % submatrixSize != 0) {
        std::cerr << "Error: the image is not decompressible since its sizes are not multiple of "
                  << submatrixSize << std::endl;
        throw std::runtime_error("Error: the image is not decompressible since its sizes are not multiple of 8");
    }

#pragma omp parallel for
    for (size_t r = 0; r < rows; r += submatrixSize) {
        for (size_t c = 0; c < cols; c += submatrixSize) {
            jpeg_decompression(r, c, submatrixSize, decompressed, Q);
        }
    }

    Image image = Image(decompressed);
    return image;
}

// #################### PRIVATE ####################

/**
 * Function that decompresses a single 8x8 block using JPEG (dequantization + inverse DCT).
 *
 * The output is directly copy into the corresponding position of decompressed.
 *
 * @param r: position in image_data of the first row of the current submatrix.
 * @param c: position in image_data of the first column of the current submatrix.
 * @param submatrixSize: block size (is a squared block).
 * @param decompressed: decompressed image matrix.
 * @param Q: quantization matrix.
*/
void CompressedImage::jpeg_decompression(
    const int r,
    const int c,
    const int submatrixSize,
    std::vector<std::vector<double>>& decompressed,
    std::vector<std::vector<double>>& Q
){
    // 1. Create a copy of the submatrix in which the value is moltiplied by the corresponding element in Q
    std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
    for (int i = 0; i < submatrixSize; ++i) {
        for (int j = 0; j < submatrixSize; ++j) {
            submatrix[i][j] = std::round(this->compressed[r+i][c+j] * Q[i][j]);
        }
    }

    // 2. Take the 2-dimensional idct of the block.
    dct::solver::InverseDiscreteCosineTransform idct_solver = dct::solver::InverseDiscreteCosineTransform();
    const dct::solver::ComputationMode cm = dct::solver::ComputationMode::OPENMP;
    idct_solver.compute(submatrix, cm);

    // 3. Copy the decompressed submatrix into the original image
    for (int i = 0; i < submatrixSize; ++i){
        for (int j = 0; j < submatrixSize; ++j){
            // 4. Add 128 from each entry, so that the entries are now again integers between 0 and 255.
            decompressed[r+i][c+j] = round(submatrix[i][j]) + 128.0;
        }
    }
}

/**
 * Function that saves a vector using RLE to a binary file.
 * The vector corresponds to zigzag scan of a submatrix.
 *
 * @param file: binary file stream.
 * @param zzVector: zigzag-scanned vector.
*/
const void CompressedImage::save_compressed_submatrix(std::ofstream& file, const std::vector<double>& zzVector) {
    std::vector<std::pair<int, int>> rleVector = RLECompressor::compress(zzVector);
    /*
    uint8_t rle_size = rle.size();
    file.write(reinterpret_cast<const char*>(&rle_size), sizeof(uint8_t));*/

    for (const auto& [repetitions, value] : rleVector) {
        int16_t count = static_cast<int16_t>(repetitions);
        if (count == 1) {
            //Write only the value and ignore repetitions
            int16_t val = static_cast<int16_t>(value);
            file.write(reinterpret_cast<const char*>(&val), sizeof(val));
        } else {
            //Write a reserved value and then write #repetitions and value
            uint8_t val = static_cast<uint8_t>(value);
            int16_t mark = static_cast<int16_t>(-1);
            file.write(reinterpret_cast<const char*>(&mark), sizeof(mark));
            file.write(reinterpret_cast<const char*>(&count), sizeof(count));
            file.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
    }

    /* different version in which the couple (count, value) are always written completly
    for (const auto& [repetitions, value] : rleVector) {
        int16_t count = static_cast<int16_t>(repetitions);
        uint8_t val = static_cast<uint8_t>(value);
        file.write(reinterpret_cast<const char*>(&count), sizeof(int16_t));
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint8_t));
    */
}

/**
 * Function that loads a compressed image from a binary file into a matrix (vector of vector).
 *
 * The binary file contains the image data in a raw format.
 *
 * @param path: path to the binary file.
 * @return: the matrix containing the image.
*/
const std::vector<std::vector<double>> CompressedImage::load_from_binary(const std::string& path){
    // Open the file in binary mode to read
    std::ifstream file(path, std::ios::binary);

    if (!file) {
        std::cerr << "Error during opening .bin file" << std::endl;
        throw std::runtime_error("Error during opening .bin file");
    }

    // Read the matrix size
    size_t rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    // Create an empty matrix with the previous size
    std::vector<std::vector<double>> img_matrix(rows, std::vector<double>(cols));

    // Read matrix elements and store them in img_matrix
    for (int r=0; r < rows; ++r) {
        for (int c=0; c < cols; ++c) {
            int8_t value;
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            img_matrix[r][c] = static_cast<double>(value);
        }
    }

    file.close();

    return img_matrix;
}

/**
 * Function that loads a compressed matrix from a compressed binary file (zigzag + rle).
 *
 * @param path: the path to the compressed binary file.
 * @return: the matrix containing the image.
 */
const std::vector<std::vector<double>> CompressedImage::load_from_compressed_binary(const std::string& path){
    std::ifstream file(path, std::ios::binary);

    // Reads the image dimensions and the submatrix size (must be 8x8).
    int rows, cols, submatrixSize;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.read(reinterpret_cast<char*>(&submatrixSize), sizeof(submatrixSize));

    if (submatrixSize != 8) {
        throw std::runtime_error(
            "Error: the compressed binary file cannot be decompressed using jpeg (submatrix size != 8)"
        );
    }

    std::vector<std::vector<double>> img_matrix(rows, std::vector<double>(cols));

    // For each 8x8 block, reads the compressed data,
    // decodes it (inverse zig-zag scan), and places it into the final image matrix.
    for (int r = 0; r < rows; r += submatrixSize) {
        for (int c = 0; c < cols; c += submatrixSize) {
            std::vector<double>  zzVector = load_compressed_submatrix(file, submatrixSize*submatrixSize);
            std::vector<std::vector<double>> submatrix = ZigZagScan::inverse_scan(
                zzVector, submatrixSize, submatrixSize
            );
            for (int i = 0; i < submatrixSize; ++i)
                for (int j = 0; j < submatrixSize; ++j)
                    img_matrix[r+i][c+j] = submatrix[i][j];
        }
    }

    file.close();

    return img_matrix;
}

/**
 * Function that loads a submatrix from the compressed binary file stream.
 *
 * It returns a vector of size vectorSize, containing the zigzag scan of the submatrix.
 *
 * @param file: reference to the input binary file stream.
 * @param vectorSize: number of elements to read in the file.
 * @return: the vector containing the zigzag scan of the submatrix.
 */
const std::vector<double> CompressedImage::load_compressed_submatrix(std::ifstream& file, const int vectorSize) {
    std::vector<std::pair<int, int>> zzVector;
    int i = 0;

    // Reads values sequentially until we fill zzVector
    while(i < vectorSize) {
        int16_t v;
        file.read(reinterpret_cast<char*>(&v), sizeof(int16_t));

        if (v != -1){
            // v is not a reserved value => it is a value with no contiguous repetitions
            zzVector.emplace_back(static_cast<long>(1), static_cast<int>(v));
            i++;
        } else {
            // we have found a reserved value => the next two value are (#repetitions, value)
            int16_t count;
            int8_t val;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));
            file.read(reinterpret_cast<char*>(&val), sizeof(val));
            zzVector.emplace_back(static_cast<long>(count), static_cast<int>(val));
            i += count+1; //i = i + #repetitions + 1
        }
    }

    return RLECompressor::decompress(zzVector);

    /* Version that read always value as couple (#repetitions, value)
    while(i < vectorSize){
        int8_t z;
        int16_t v;
        file.read(reinterpret_cast<char*>(&z), sizeof(int8_t));
        file.read(reinterpret_cast<char*>(&v), sizeof(int16_t));
        zzVector.emplace_back(static_cast<int>(z), static_cast<double>(v));
        i += z+1; //i = i + #zeros + 1
    }

    return RLECompressor::decompress(zzVector);

    //1st version
    uint8_t rle_size;
    file.read(reinterpret_cast<char*>(&rle_size), sizeof(uint8_t));

    std::vector<std::pair<int, int>> zz_submatrix;
    for (int i = 0; i < rle_size; ++i) {
        int8_t z;
        int16_t v;
        file.read(reinterpret_cast<char*>(&z), sizeof(int8_t));
        file.read(reinterpret_cast<char*>(&v), sizeof(int16_t));
        rle[i] = {z, v};
    }

    return RLECompressor::decompress(dc, rle);*/
}