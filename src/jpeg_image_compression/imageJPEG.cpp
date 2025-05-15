#include <iostream>
#include <fstream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "jpeg_image_compression/imageJPEG.hpp"
#include "zigzag_scan/zigzag_scan.hpp"
#include "rle_compressor/rle_compressor.hpp"
#include "discrete-cosine-transform-solver/discrete-cosine-transform/discrete-cosine-transform.hpp"
#include "discrete-cosine-transform-solver/inverse-discrete-cosine-transform/inverse-discrete-cosine-transform.hpp"

//#################### CONSTRUCTORS ##############################################

/*
 * Constructor that initializes the ImageJPEG object from a given matrix by copying it.
 * @params inputMatrix: 2D vector containing pixel values.
 */
ImageJPEG::ImageJPEG(std::vector<std::vector<double>> inputMatrix): image(inputMatrix) {}

/*
 * Constructor that loads a PNG file into the variable image of ImageJPEG object.
 * @input_image_path: path to the PNG image file.
 */
ImageJPEG::ImageJPEG(const char* image_path){
    this->image = load_from_png(image_path);
};

/*
 * Constructor that loads the compressed image from a file .bin
 * @compressed_image_path:  path to binary file;
 * @option: 1 (raw binary), 2 (compressed binary).
 */
ImageJPEG::ImageJPEG(const std::string compressed_image_path, const int option){
    if (option == 1){
        this->compressed = load_from_binary(compressed_image_path);
    }
    else if (option == 2){
        this->compressed = load_from_compressed_binary(compressed_image_path);
    }
    else{
        throw std::runtime_error("Error: invalid option in ImageJPEG constructor. Acceptable ones are only option=1 for \"load compressed image from a binary file\", or option=2 for \"load compressed image form a compressed binary file (zigzag+rle)\"");
    }
};

//###################### PUBLIC ##########################################

/* Function that saves the image matrix of the current object as a PNG file.
 * @path: output path for the PNG file.
 */
const void ImageJPEG::save_image_as_png(const std::string path){
    if(image.empty()){
        throw std::invalid_argument("Error: there is no image to save as png.");
    }
    save_as_png(this->image, path);
}

/* Function that saves the decompressed matrix of the current object as a PNG file.
 * @path: output path for the PNG file.
 */
const void ImageJPEG::save_decompressed_as_png(const std::string path){
    if(decompressed.empty()){
        throw std::invalid_argument("Error: there is no decompressed image to save as png.");
    }
    save_as_png(this->decompressed, path);
}

/* Function that saves the compressed image as a binary file.
 * @path: output path for the binary file.
 */
const void ImageJPEG::save_compressed_as_binary(const std::string& path){
    if(compressed.empty()){
        throw std::invalid_argument("Error: there is no compressed image to save as binary file.");
    }
    save_as_binary(this->compressed, path);

    std::cout << "Compressed image written successfully in a binary file!" << std::endl;
}

/* Function that saves the compressed image as a compressed binary file using zigzag + RLE.
 * @path: output path for the compressed binary file.
 */
const void ImageJPEG::save_compressed_as_compressed_binary(const std::string& path){
    if(compressed.empty()){
        throw std::invalid_argument("Error: there is no compressed image to save as binary file.");
    }
    save_as_compressed_binary(this->compressed, path);

    std::cout << "Image matrix written successfully in a binary file using zigzag scan & rle compression!" << std::endl;
}

/*
=> Meglio tenere il costruttore perchè altrimenti potrei avere un oggetto imageJPEG in cui decompressed non corrisponde ad image
esempio: creo oggetto imageJPEG a partire da image, poi carico compressed con una matrice contenuta in un file binario che non centra nulla con image
void ImageJPEG::load_compressed_from_binary(const std::string& path){
    this->compressed = load_from_binary(compressed_image_path);
}

void ImageJPEG::load_compressed_from_compressed_binary(const std::string& path){
    this->compressed = load_from_compressed_binary(compressed_image_path);
}
*/

/*
 * Function that implements the JPEG compression algorithm on the full image.
 */
void ImageJPEG::compress(){
    std::cout<< "Start compress()" << std::endl;
    if(image.empty()){
        throw std::invalid_argument("Error: there is no image to compress.");
    }

    const size_t rows = this->image.size();
    const size_t cols = this->image[0].size();
    this->compressed = std::vector<std::vector<double>>(rows, std::vector<double>(cols));

    // Create the constant matrix Q (quantization matrix) --> is a 8x8
    //FIXME: maybe initialize Q as a global variable
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
    if(rows%submatrixSize!=0 || cols%submatrixSize!=0){
        std::cout<<"Error: the image is not compressible since its sizes are not multiple of " << submatrixSize << std::endl;
        throw std::runtime_error("Error: the image is not compressible since its sizes are not multiple of 8");
        return;
    }

    #pragma omp parallel for
    for (int r = 0; r < rows; r += submatrixSize) {
        for (int c = 0; c < cols; c += submatrixSize) {
            jpeg_compression(r, c, submatrixSize, Q);
        }
    }
}

/*
 * Function that implements the jpeg decompression algorithm on compressed
 */
void ImageJPEG::decompress(){
    if(compressed.empty()){
        throw std::invalid_argument("Error: there is no compresed image to decompress.");
    }
    const size_t rows = compressed.size();
    const size_t cols = compressed[0].size();
    this->decompressed = std::vector<std::vector<double>>(rows, std::vector<double>(cols));

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
    if(rows%submatrixSize!=0 || cols%submatrixSize!=0){
        std::cout<<"Error: the image is not decompressible since its sizes are not multiple of " << submatrixSize << std::endl;
        throw std::runtime_error("Error: the image is not decompressible since its sizes are not multiple of 8");
        return;
    }

    #pragma omp parallel for
    for (size_t r = 0; r < rows; r += submatrixSize) {
        for (size_t c = 0; c < cols; c += submatrixSize) {
            jpeg_decompression(r, c, submatrixSize, Q);
        }
    }
}


//##################### PRIVATE #################################

/*
 * Function that compresses a single 8x8 block using JPEG (DCT + quantization)
 * The output is directly saved into the corresponding position of compressed.
 * @r: position in image of the first row of the current submatrix;
 * @c: position in image of the first column of the current submatrix;
 * @submatrixSize: block size (is a squared block);
 * @Q: quantization matrix.
 */
void ImageJPEG::jpeg_compression(const int r, const int c, const int submatrixSize, std::vector<std::vector<double>>& Q){
    // 1. Create a copy of the submatrix
    std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            // 2. Subtract 128 from each entry, so that the entries are now integers between -128 and 127.
            submatrix[i][j] = this->image[r+i][c+j] - 128.0;
        }
    }

    // 3. Take the 2-dimensional discrete cosine transform (dct) of the block.
    dct::solver::DiscreteCosineTransform dct_solver = dct::solver::DiscreteCosineTransform();
    const dct::solver::ComputationMode cm = dct::solver::ComputationMode::OPENMP;
    dct_solver.compute(submatrix, cm);

    // 4. Divide the block elementwise by a constant matrix Q known as a quantization matrix, and then round each entry to the nearest integer
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            // 5. Save the result in the big matrix of the image
            this->compressed[r+i][c+j] = std::round(submatrix[i][j] / Q[i][j]);
        }
    }
}

/*
 * Function that decompresses a single 8x8 block using JPEG (dequantization + inverse DCT).
 * The output is directly copy into the corresponding position of image.
 * @r: position in image_data of the first row of the current submatrix;
 * @c: position in image_data of the first column of the current submatrix;
 * @submatrixSize: block size (is a squared block);
 * @Q: quantization matrix.
*/
void ImageJPEG::jpeg_decompression(const int r, const int c, const int submatrixSize, std::vector<std::vector<double>>& Q){
    // 1. Create a copy of the submatrix in which the value is moltiplied by the corresponding element in Q
    std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            submatrix[i][j] = std::round(this->compressed[r+i][c+j] * Q[i][j]);
        }
    }

    // 2. Take the 2-dimensional idct of the block.
    dct::solver::InverseDiscreteCosineTransform idct_solver = dct::solver::InverseDiscreteCosineTransform();
    const dct::solver::ComputationMode cm = dct::solver::ComputationMode::OPENMP;
    idct_solver.compute(submatrix, cm);

    // 3. Copy the decompressed submatrix into the original image
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            // 4. Add 128 from each entry, so that the entries are now again integers between 0 and 255.
            this->decompressed[r+i][c+j] = round(submatrix[i][j]) + 128.0;
        }
    }
}

/*
 * Function that saves a matrix as a PNG file using stb_image_write.
 * @img_matrix: matrix to save; path: output PNG path.
 */
const void ImageJPEG::save_as_png(const std::vector<std::vector<double>>& img_matrix, const std::string path){
    const size_t rows = img_matrix.size();
    const size_t cols = img_matrix[0].size();

    // Convert the image into grayscale (pixel values between 0,255) and store it as 1D vector
    std::vector<unsigned char> image_data(rows * cols);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            image_data[r * cols + c] = std::min(255.0, std::max(0.0, img_matrix[r][c])); //clamp value between 0,255
        }
    }
    // Save the image using stb_image_write
    if (stbi_write_png(path.c_str(), cols, rows, 1, image_data.data(), cols) == 0) {
        std::cerr << "Error: Could not save grayscale image" << std::endl;
        throw std::runtime_error("Error: Could not save grayscale image");
        return;
    }

    std::cout << "Image written successfully in a png file!" << std::endl;
}

/*
 * Function that saves a matrix as a binary file.
 * @img_matrix: matrix;
 * @path: binary file path.
*/
const void ImageJPEG::save_as_binary(std::vector<std::vector<double>>& img_matrix, const std::string& path){
    // Open the file in bynary mode to write
    std::ofstream file(path, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening binary file!" << std::endl;
        throw std::runtime_error("Error opening binary file!");
    }

    // Write matrix size at the beginning of the binary file (rows, cols)
    size_t rows = img_matrix.size();
    size_t cols = img_matrix[0].size();

    file.write(reinterpret_cast<char*>(&rows), sizeof(rows)); //reinterpret_cast<char*> --> to interpret each double as an array of byte
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));

    // Write matrix's elements
    for (int r=0; r<rows; ++r) {
        for (int c=0; c<cols; ++c) {
            int8_t value = static_cast<int8_t>(img_matrix[r][c]); // since img_matrix[r][c] stays in a range of -128 e 127
            file.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
    }

    file.close();
}

/* Function that saves a matrix as a compressed binary file (with zigzag + RLE).
 * @img_matrix: compressed matrix;
 * @path: compressed binary file path.
*/
const void ImageJPEG::save_as_compressed_binary(std::vector<std::vector<double>>& img_matrix, const std::string& path){
    int submatrixSize = 8;

    std::ofstream file(path, std::ios::binary);

    int rows = img_matrix.size();
    int cols = img_matrix[0].size();
    file.write(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.write(reinterpret_cast<char*>(&submatrixSize), sizeof(submatrixSize));

    for (int r = 0; r < rows; r += submatrixSize) {
        for (int c = 0; c < cols; c += submatrixSize) {
            // Create a copy of the submatrix and copy inside it the correspondin value of img_matrix
            std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
            for (int i = 0; i < submatrixSize; ++i)
                for (int j = 0; j < submatrixSize; ++j)
                    submatrix[i][j] = img_matrix[r+i][c+j];
            // Apply zigzag scan to the submatrix
            std::vector<double> zzVector = ZigZagScan::scan(submatrix);
            //save the submatrix in the binary file (using rle compression)
            save_compressed_submatrix(file, zzVector);
        }
    }

    file.close();
}

/* Function that saves a vector using RLE to a binary file. The vector corresponds to zigzag scan of a submatrix.
 * @file: binary file stream;
 * @zzVector: zigzag-scanned vector.
*/
const void ImageJPEG::save_compressed_submatrix(std::ofstream& file, const std::vector<double>& zzVector) {
    std::vector<std::pair<int, int>> rleVector = RLEcompressor::compress(zzVector);
    /*
    uint8_t rle_size = rle.size();
    file.write(reinterpret_cast<const char*>(&rle_size), sizeof(uint8_t));*/

    for (const auto& [repetitions, value] : rleVector) {
        auto count = static_cast<int16_t>(repetitions);
        if (count == 1){
            //Write only the value and ignore repetitions
            auto val = static_cast<int16_t>(value);
            file.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
        else{
            //Write a reserved value and then write #repetitions and value
            auto val = static_cast<uint8_t>(value);
            auto mark = static_cast<int16_t>(-1);
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

/*
 * Function that loads an image from a PNG file into a matrix using stb_image.
 * @image_path: path to the PNG file.
*/
const std::vector<std::vector<double>> ImageJPEG::load_from_png(const char* image_path){
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

    stbi_image_free(image_data); //FREE MEMORY!!

    return image_matrix;
}

/*
 * Function that Loads a compressed image from a binary file into a matrix (vector of vector).
 * @path: path to the binary file.
*/
const std::vector<std::vector<double>> ImageJPEG::load_from_binary(const std::string path){
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

/*
 * Function that loads a compressed matrix from a compressed binary file (zigzag + rle).
 * @path: the path to the compressed binary file
 */
const std::vector<std::vector<double>> ImageJPEG::load_from_compressed_binary(const std::string& path){
    std::ifstream file(path, std::ios::binary);

    // Reads the image dimensions and the submatrix size (must be 8x8).
    int rows, cols, submatrixSize;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    file.read(reinterpret_cast<char*>(&submatrixSize), sizeof(submatrixSize));

    if(submatrixSize != 8){
        throw std::runtime_error("Error: the compressed binary file cannot be decompressed using jpeg (submatrix size != 8)");
    }

    std::vector<std::vector<double>> img_matrix(rows, std::vector<double>(cols));

    // For each 8x8 block, reads the compressed data, decodes it (inverse zig-zag scan), and places it into the final image matrix.
    for (int r = 0; r < rows; r += submatrixSize) {
        for (int c = 0; c < cols; c += submatrixSize) {
            std::vector<double>  zzVector = load_compressed_submatrix(file, submatrixSize*submatrixSize);
            std::vector<std::vector<double>> submatrix = ZigZagScan::inverse_scan(zzVector, submatrixSize, submatrixSize);
            for (int i = 0; i < submatrixSize; ++i)
                for (int j = 0; j < submatrixSize; ++j)
                    img_matrix[r+i][c+j] = submatrix[i][j];
        }
    }

    file.close();

    return img_matrix;
}

/* Function that loads a submatrix from the compressed binary file stream.
 * It returns a vector of size vectorSize, containing the zigzag scan of the submatrix
 * @file: reference to the input binary file stream;
 * @vectorSize: number of elements to read in the file.
 */
const std::vector<double> ImageJPEG::load_compressed_submatrix(std::ifstream& file, const int vectorSize) {
    std::vector<std::pair<int, int>> zzVector;
    int i = 0;

    // Reads values sequentially untill we fill zzVector
    while(i < vectorSize){
        int16_t v;
        file.read(reinterpret_cast<char*>(&v), sizeof(int16_t));

        if (v != -1){
            // v is not a reserved value => it is a value with no contiguous repetitions
            zzVector.emplace_back(static_cast<long>(1), static_cast<int>(v));
            i++;
        }
        else{
            //we have found a reserved value => the next two value are (#repetitions, value)
            int16_t count;
            int8_t val;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));
            file.read(reinterpret_cast<char*>(&val), sizeof(val));
            zzVector.emplace_back(static_cast<long>(count), static_cast<int>(val));
            i += count+1; //i = i + #repetitions + 1
        }
    }

    return RLEcompressor::decompress(zzVector);

    /* Version that read always valu as couple (#repetitions, value)
    while(i < vectorSize){
        int8_t z;
        int16_t v;
        file.read(reinterpret_cast<char*>(&z), sizeof(int8_t));
        file.read(reinterpret_cast<char*>(&v), sizeof(int16_t));
        zzVector.emplace_back(static_cast<int>(z), static_cast<double>(v));
        i += z+1; //i = i + #zeros + 1
    }

    return RLEcompressor::decompress(zzVector);

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

    return RLEcompressor::decompress(dc, rle);*/
}