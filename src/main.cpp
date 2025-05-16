#include <iostream>
#include "config_loader/json-configuration-loader.hpp"
#include "fourier-transform-solver/fast-fourier-transform/fast-fourier-transform.hpp"
#include "fourier-transform-solver/inverse-fast-fourier-transform/inverse-fast-fourier-transform.hpp"
#include "matplot/matplot.h"
#include "signal-generator/space-domain-signal-generator.hpp"
#include "signal-generator/time-domain-signal-generator.hpp"
#include "signal_saver/csv-signal-saver.hpp"

/**
 * Environment variable name for the file path for the configuration file.
 */
#define ENV_FILE_PATH "CONFIG_FILE_PATH_FFT"

//#########################
// from https://github.com/nothings/stb/tree/master
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//TODO: dovremmo salvare le varie trasformate in una variabile a parte (da passare poi nel return) e non sovrascrivere sempre i dati originali
//VERSIONE ZIGZAG + RLE di JPG basato su DCT

std::vector<int> zigzag_order = {
     0,  1,  5,  6, 14, 15, 27, 28,
     2,  4,  7, 13, 16, 26, 29, 42,
     3,  8, 12, 17, 25, 30, 41, 43,
     9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

std::vector<int16_t> zigzag_scan(const std::vector<std::vector<double>>& block) {
    std::vector<int16_t> result(64);
    for (int idx = 0; idx < 64; ++idx) {
        int i = zigzag_order[idx] / 8;
        int j = zigzag_order[idx] % 8;
        result[idx] = static_cast<int16_t>(block[i][j]);
    }
    return result;
}

std::vector<std::vector<double>> inverse_zigzag(const std::vector<int16_t>& zz) {
    std::vector<std::vector<double>> block(8, std::vector<double>(8));
    for (int i = 0; i < 64; ++i) {
        int row = zigzag_order[i] / 8;
        int col = zigzag_order[i] % 8;
        block[row][col] = zz[i];
    }
    return block;
}

std::vector<std::pair<int, int16_t>> rle_encode(const std::vector<int16_t>& input) {
    std::vector<std::pair<int, int16_t>> encoded; // (#zeri_precedenti, valore)
    int zero_count = 0;

    for (size_t i = 1; i < input.size(); ++i) { // partiamo da 1, DC è separato
        if (input[i] == 0) {
            zero_count++;
        } else {
            encoded.emplace_back(zero_count, input[i]);
            zero_count = 0;
        }
    }

    // End-of-block: zeri rimanenti
    if (zero_count > 0) {
        encoded.emplace_back(0, 0); // JPEG usa (0,0) come fine blocco
    }

    return encoded;
}

std::vector<int16_t> rle_decode(int16_t dc, const std::vector<std::pair<int8_t, int16_t>>& encoded) {
    std::vector<int16_t> decoded;
    decoded.push_back(dc); // DC

    for (const auto& [zeros, value] : encoded) {
        if (zeros == 0 && value == 0) break; // End-of-block
        decoded.insert(decoded.end(), zeros, 0);
        decoded.push_back(value);
    }

    while (decoded.size() < 64) decoded.push_back(0); // Pad with zeros
    return decoded;
}

void save_compressed_block(std::ofstream& file, const std::vector<int16_t>& zz_block) {
    file.write(reinterpret_cast<const char*>(&zz_block[0]), sizeof(int16_t)); // DC

    auto rle = rle_encode(zz_block);
    uint8_t rle_size = rle.size();
    file.write(reinterpret_cast<const char*>(&rle_size), sizeof(uint8_t));

    for (const auto& [zeros, val] : rle) {
        int8_t z = zeros;
        file.write(reinterpret_cast<const char*>(&z), sizeof(int8_t));
        file.write(reinterpret_cast<const char*>(&val), sizeof(int16_t));
    }
}

std::vector<int16_t> load_compressed_block(std::ifstream& file) {
    int16_t dc;
    file.read(reinterpret_cast<char*>(&dc), sizeof(int16_t));

    uint8_t rle_size;
    file.read(reinterpret_cast<char*>(&rle_size), sizeof(uint8_t));

    std::vector<std::pair<int8_t, int16_t>> rle(rle_size);
    for (int i = 0; i < rle_size; ++i) {
        int8_t z;
        int16_t v;
        file.read(reinterpret_cast<char*>(&z), sizeof(int8_t));
        file.read(reinterpret_cast<char*>(&v), sizeof(int16_t));
        rle[i] = {z, v};
    }

    return rle_decode(dc, rle);
}

std::vector<std::vector<double>> load_image_from_compressed_binary(const std::string compressed_image_path){
    std::ifstream file("compressed_image.bin", std::ios::binary);
    size_t rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    std::vector<std::vector<double>> img_matrix(rows, std::vector<double>(cols));

    for (int r = 0; r < rows; r += 8) {
        for (int c = 0; c < cols; c += 8) {
            auto zz = load_compressed_block(file);
            auto block = inverse_zigzag(zz);
            for (int i = 0; i < 8; ++i)
                for (int j = 0; j < 8; ++j)
                    img_matrix[r+i][c+j] = block[i][j];
        }
    }
    file.close();

    return img_matrix;
}

void save_compressed_img_as_compressed_binary_file(std::vector<std::vector<double>> img_matrix, const std::string compressed_image_path){
    std::ofstream file("compressed_image.bin", std::ios::binary);
    size_t rows = img_matrix.size(), cols = img_matrix[0].size();
    file.write(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));

    for (int r = 0; r < rows; r += 8) {
        for (int c = 0; c < cols; c += 8) {
            std::vector<std::vector<double>> block(8, std::vector<double>(8));
            for (int i = 0; i < 8; ++i)
                for (int j = 0; j < 8; ++j)
                    block[i][j] = img_matrix[r+i][c+j];
            auto zz = zigzag_scan(block);
            save_compressed_block(file, zz);
        }
    }

    file.close();

    std::cout << "Compressed image written successfully in a binary file!" << std::endl;
}

/*
Function that computes the Discrete Cosine Transform 1D
- data --> vector on which we want compute the DCT
*/
std::vector<double> compute_dct(std::vector<double>& data) {
    const size_t N = data.size();
    std::vector<double> dct_coefficients(N);

    for (int k=0; k<N; ++k) {
        double sum = 0.0;
        for (int n=0; n<N; ++n) {
            sum += data[n] * std::cos((M_PI*(2*n+1)*k)/(2*N));
        }

        if (k==0) {
            dct_coefficients[k] = std::sqrt(1.0/N) * sum; // alpha = sqrt(1/N)
        }
        else {
            dct_coefficients[k] = std::sqrt(2.0/N) * sum; // alpha = sqrt(2/N)
        }
    }

    return dct_coefficients;
}

/*
Function that computes the Inverse Discrete Cosine Transform 1D
- dct_coefficients --> vector containg the Discrete Cosine Transform components
*/
std::vector<double> compute_idct(std::vector<double> dct_coefficients) {
    const size_t N = dct_coefficients.size();
    std::vector<double> data(N);

    for (int n=0; n<N; ++n) {
        double sum = 0.0;
        for (int k=0; k<N; ++k) {
            if (k==0) {
                sum += std::sqrt(1.0/N) * dct_coefficients[k]; //because the first element of dct has a different scaled factor
            }
            else {
                sum += std::sqrt(2.0/N) * dct_coefficients[k] * std::cos((M_PI * (2*n+1)*k)/(2.0*N));
            }
        }
        data[n] = sum;
    }

    return data;
}

/*
Function that computes the Discrete Cosine Transform 2D
- data --> matrix on which we want compute the DCT 2D
*/
void compute_dct_2D(std::vector<std::vector<double>>& data) {
    const size_t rows = data.size();
    const size_t cols = data[0].size();
    //std::vector<std::vector<<double>> dct_coefficients(rows,std::vector<double>(cols));

    // Apply 1D DCT to each row
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i) {
        //dct_coefficients[i] = compute_dct[data[i]];
        data[i] = compute_dct(data[i]);
    }

    // Transpose the matrix
    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            //transposed[j][i] = dct_coefficients[i][j];
            transposed[j][i] = data[i][j];


    // Apply 1D DCT to each column (originally a row in transposed)
    #pragma omp parallel for
    for (size_t i = 0; i < cols; ++i) { //NB: not rows because rows and cols refer to the original image and we do not update them after the transpose
        //transposed[i] = compute_dct(transposed[i]);
        transposed[i] = compute_dct(transposed[i]);
    }

    // Transpose back to original orientation
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            //dct_coefficients[i][j] = transposed[j][i];
            data[i][j] = transposed[j][i];

}

/*
Function that computes the Inverse Discrete Cosine Transform 2D
- dct --> matrix containg the Discrete Cosine Transform components
*/
void compute_idct_2D(std::vector<std::vector<double>>& dct) {
    const size_t rows = dct.size();
    const size_t cols = dct[0].size();

    // Apply 1D IDCT to each row
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i) {
        dct[i] = compute_idct(dct[i]);
    }

    // Transpose the matrix
    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            transposed[j][i] = dct[i][j];


    // Apply 1D IDCT to each column (originally a row in transposed)
    #pragma omp parallel for
    for (size_t i = 0; i < cols; ++i) { //NB: not rows because rows and cols refer to the original image and we do not update them after the transpose
        transposed[i] = compute_idct(transposed[i]);
    }

    // Transpose back to original orientation
    #pragma omp parallel for
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            dct[i][j] = transposed[j][i];
}

/*
Function that loads the image using stb_image.h
- input_image_path --> path of the image to be loaded
*/
std::vector<std::vector<double>> load_image(const char* input_image_path){
    // Load the image using stb_image
    int width, height, channels;
    unsigned char* image_data = stbi_load(input_image_path, &width, &height, &channels, 1);

    if (!image_data) {
        std::cerr << "Error: Could not load image " << input_image_path << std::endl;
        //TODO segnala errore
    }

    std::cout << "Image loaded: " << width << "x" << height << " with " << channels << " channels." << std::endl;

    // Create the vector of vector of complex number that represent the image data
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
Function that exports the image using stb_image_write.h
- img_matrix --> image to be saved
*/
void save_image(const std::vector<std::vector<double>>& img_matrix, const std::string output_image_path){
    const size_t rows = img_matrix.size();
    const size_t cols = img_matrix[0].size();

    // Convert the complex image into grayscale (pixel values between 0,255) and store it as 1D vector
    std::vector<unsigned char> image_data(rows * cols);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            image_data[r * cols + c] = std::min(255.0, std::max(0.0, img_matrix[r][c])); //clamp value between 0,255
        }
    }

    // Save the image using stb_image_write
    if (stbi_write_png(output_image_path.c_str(), cols, rows, 1, image_data.data(), cols) == 0) {
        std::cerr << "Error: Could not save grayscale image" << std::endl;
        return;
    }
}

/*
Function that reads the compressed image from binary file and saves it in a matrix (vectors of vectors) of complex value
- compressed_image_path --> path of the binary file containing the compressed image
*/
std::vector<std::vector<double>> load_image_from_binary(const std::string compressed_image_path){
    // Open the file in binary mode to read
    std::ifstream file(compressed_image_path, std::ios::binary);

    if (!file) {
        std::cerr << "Errore nell'apertura del file!" << std::endl;
        //TODO: return an error
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
            //double value;
            //file.read(reinterpret_cast<char*>(&value), sizeof(value));
            //img_matrix[r][c] = value;
            int8_t value;
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            img_matrix[r][c] = static_cast<double>(value);
        }
    }

    file.close();

    return img_matrix;
}

/*
Function that saves the compressed image in a binary file
- img_matrix --> matrix containing the compressed version of the image
- compressed_image_path --> path of the binary file in which save complex_img
*/
void save_compressed_img_as_binary_file(std::vector<std::vector<double>> img_matrix, const std::string compressed_image_path){
    // Open the file in bynary mode to write
    std::ofstream file(compressed_image_path, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening binary file!" << std::endl;
        //TODO: return an error
    }

    // Write matrix size at the beginning of the binary file (rows, cols)
    size_t rows = img_matrix.size();
    size_t cols = img_matrix[0].size();

    file.write(reinterpret_cast<char*>(&rows), sizeof(rows)); //reinterpret_cast<char*> --> to interpret each double as an array of byte
    file.write(reinterpret_cast<char*>(&cols), sizeof(cols));

    // Write matrix's elements (as complex number, so first real part and then imaginary part)
    for (int r=0; r<rows; ++r) {
        for (int c=0; c<cols; ++c) {
            //double value = img_matrix[r][c];
            int8_t value = static_cast<int8_t>(img_matrix[r][c]); // since img_matrix[r][c] stays in a range of -128 e 127
            file.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
    }

    file.close();

    std::cout << "Compressed image written successfully in a binary file!" << std::endl;
}

/*
Function that implements the jpg compression of a single submatrix.
The output is directly copy into the corresponding position of image_data.
- image_data --> contains the original image (the one that will be compressed)
- r --> position in image_data of the first row of the current submatrix
- c --> position in image_data of the first column of the current submatrix
- submatrixSize --> size of the submatrix (is a squared matrix)
- Q --> quantization matrix
*/
void jpg_compression(std::vector<std::vector<double>>& img_matrix, int r, int c, int submatrixSize, std::vector<std::vector<double>> Q){
    // 1. Create a copy of the submatrix
    std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            // 2. Subtract 128 from each entry, so that the entries are now integers between -128 and 127.
            submatrix[i][j] = img_matrix[r+i][c+j] - 128.0;
        }
    }

    // 3. Take the 2-dimensional discrete cosine transform (dct) of the block.
    compute_dct_2D(submatrix);

    // 4. Divide the block elementwise by a constant matrix Q known as a quantization matrix, and then round each entry to the nearest integer
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            // 5. Save the result in the big matrix of the image
            img_matrix[r+i][c+j] = std::round(submatrix[i][j] / Q[i][j]);
        }
    }
}

/*
Function that implements the jpg compression algorithm on the hole image
- input_image_path --> it is the path of the image to be compressed
*/
void compress_image(const char* input_image_path){
    // Load the image
    std::vector<std::vector<double>> img_matrix = load_image(input_image_path);
    const char* path="original_image.png";
    save_image(img_matrix,path);

    const size_t rows = img_matrix.size();
    const size_t cols = img_matrix[0].size();

    std::cout<< rows << "x" << cols << std::endl;

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
        std::cout<<"L'immagine non è comprimibile in quanto le dimensioni non sono multipli di " << submatrixSize << std::endl;
        return;
    }

    #pragma omp parallel for
    for (int r = 0; r < rows; r += submatrixSize) {
        for (int c = 0; c < cols; c += submatrixSize) {
            jpg_compression(img_matrix, r, c, submatrixSize, Q);
        }
    }

    //Save the compressed image
    const std::string output_image_path = "compressed_image.bin"; //FIXME: generalize it
    save_compressed_img_as_binary_file(img_matrix, output_image_path);
    //save_compressed_img_as_compressed_binary_file(img_matrix, output_image_path); //zigzag + rle
}

/*
Function that implements the jpg decompression of a single submatrix.
The output is directly copy into the corresponding position of image_data.
- img_matrix --> contains the image (the one that will be decompressed)
- r --> position in image_data of the first row of the current submatrix
- c --> position in image_data of the first column of the current submatrix
- submatrixSize --> size of the submatrix (is a squared matrix)
- Q --> quantization matrix
*/
void jpg_decompression(std::vector<std::vector<double>>& img_matrix, int r, int c, int submatrixSize, std::vector<std::vector<double>> Q){
    // 1. Create a copy of the submatrix in which the value is moltiplied by the corresponding element in Q
    std::vector<std::vector<double>> submatrix(submatrixSize, std::vector<double>(submatrixSize));
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            submatrix[i][j] = std::round(img_matrix[r+i][c+j] * Q[i][j]);
        }
    }

    // 2. Take the 2-dimensional idct of the block.
    compute_idct_2D(submatrix);

    // 3. Copy the decompressed submatrix into the original image
    for (int i=0; i<submatrixSize; ++i){
        for (int j=0; j<submatrixSize; ++j){
            // 4. Add 128 from each entry, so that the entries are now again integers between 0 and 255.
            img_matrix[r+i][c+j] = submatrix[i][j] + 128.0;
        }
    }
}

/*
Function that implements the jpg decompression algorithm on a binary file and it saves the reulting decompressed image.
- compressed_image_path --> it is the path of the binary file containing the compressed image
*/
void decompress_image(const char* compressed_image_path){
    // Load the image
    std::vector<std::vector<double>> img_matrix = load_image_from_binary(compressed_image_path);
    //std::vector<std::vector<double>> img_matrix = load_image_from_compressed_binary(compressed_image_path); //zigzag + rle

    const size_t rows = img_matrix.size();
    const size_t cols = img_matrix[0].size();

    std::cout<< rows << "x" << cols << std::endl;

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
        std::cout<<"L'immagine non è decomprimibile in quanto le dimensioni non sono multipli di " << submatrixSize << std::endl;
        return;
    }

    #pragma omp parallel for
    for (size_t r = 0; r < rows; r += submatrixSize) {
        for (size_t c = 0; c < cols; c += submatrixSize) {
            jpg_decompression(img_matrix, r, c, submatrixSize, Q);
        }
    }

    //Save the decompressed image
    const std::string output_image_path = "decompressed_image.png"; //FIXME: generalize it
    save_image(img_matrix, output_image_path);
}

//######################################


int main() {
    /*// ============================================= Configuration Loading =============================================
    // get the file path from environment variable
    if (getenv(ENV_FILE_PATH) == nullptr) {
        std::cerr << "Warning: Environment variable " << ENV_FILE_PATH << " is not set. "
                                                                          "Using default configuration file path.\n";
        // if the environment variable is not set, use the default configuration file path
        setenv(ENV_FILE_PATH, "resources/sample-config.json", 0);
    }
    const std::string filePath = getenv(ENV_FILE_PATH);

    // load the configuration from the file
    const auto loader = new JSONConfigurationLoader();
    loader->loadConfigurationFromFile(filePath);
    const auto json_loaded = new JsonFieldHandler(loader->getConfigurationData());
    // free unused memory
    delete loader;
    // get the simulation parameters
    const int signal_length = json_loaded->getSignalLength();
    const double *frequency = new double(json_loaded->getHzFrequency());
    const double *phase = new double(json_loaded->getPhase());
    const double *noise = new double(json_loaded->getNoise());
    const std::string *signal_domain = new std::string(json_loaded->getSignalDomain());
    // get the seed if it exists
    const std::optional<int> seed = json_loaded->hasSeed() ? std::optional(json_loaded->getSeed()) : std::nullopt;
    // prepare the signal saver and use the unique pointer to manage the memory
    const auto csv_signal_saver = std::make_unique<CsvSignalSaver>();
    // free unused memory
    delete json_loaded;


    // ================================================ Generate Signal ================================================
    // generate the signal
    std::vector<std::complex<double>> signal(signal_length);
    if (*signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        TimeDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, *frequency, *phase, *noise);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        SpaceDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, *frequency, *phase, *noise);
    }
    // free unused memory
    delete frequency;
    delete phase;
    delete noise;
    delete signal_domain;
    // and save it to a file
    csv_signal_saver->saveToFile(signal, "output/input_signal");
    // since the transformation is in-place, we need to keep a copy of the original signal
    std::vector<std::complex<double>> original_signal(signal_length);
    std::vector<std::complex<double>> sequential_fft_input(signal_length);
    std::vector<std::complex<double>> parallel_fft_input(signal_length);
    std::vector<std::complex<double>> parallel_inverse_fft_input(signal_length);
    std::vector<std::complex<double>> sequential_inverse_fft_input(signal_length);
    for (size_t i = 0; i < signal_length; ++i) {
        original_signal[i] = signal[i];
        sequential_fft_input[i] = signal[i];
        parallel_fft_input[i] = signal[i];
        parallel_inverse_fft_input[i] = signal[i];
        sequential_inverse_fft_input[i] = signal[i];
    }


    // ================================================ Sequential FFT ================================================
    printf("\n\nFFT\n");
    auto *solver = new Sequential1DFastFT(sequential_fft_input);
    solver->compute();
    const std::vector<std::complex<double>> &solution = solver->getSolution();
    csv_signal_saver->saveToFile(solution, "output/fft_signal");
    // free the memory
    delete solver;


    // ================================================= Parallel FFT =================================================
    printf("\n\nParallel - FFT\n");
    auto *parallel_solver = new Parallel1DFastFT(parallel_fft_input);
    parallel_solver->compute();
    const std::vector<std::complex<double>> &parallel_solution = parallel_solver->getSolution();
    csv_signal_saver->saveToFile(parallel_solution, "output/parallel_fft_signal");
    // free the memory
    delete parallel_solver;

    // ============================================ Sequential Inverse FFT ============================================
    printf("\n\nSequential - Inverse FFT\n");
    auto *tmp_solver_seq = new Sequential1DFastFT(sequential_inverse_fft_input);
    tmp_solver_seq->compute();
    // use the FFT result as input
    auto *inverse_solver_seq = new Sequential1DInverseFastFT(tmp_solver_seq->getSolution());
    inverse_solver_seq->compute();
    const std::vector<std::complex<double>> inverse_solution_seq = inverse_solver_seq->getSolution();
    csv_signal_saver->saveToFile(inverse_solution_seq, "output/seq_inverse_fft_signal");
    // free the memory
    delete inverse_solver_seq;
    delete tmp_solver_seq;


    // ============================================= Parallel Inverse FFT =============================================
    printf("\n\nParallel - Inverse FFT\n");
    auto *tmp_solver = new Sequential1DFastFT(parallel_inverse_fft_input);
    tmp_solver->compute();
    // use the FFT result as input
    auto *inverse_solver = new Parallel1DInverseFastFT(tmp_solver->getSolution());
    inverse_solver->compute();
    const std::vector<std::complex<double>> inverse_solution = inverse_solver->getSolution();
    csv_signal_saver->saveToFile(inverse_solution, "output/parallel_inverse_fft_signal");
    // free the memory
    delete inverse_solver;
    delete tmp_solver;


    // =================================================== Plotting ===================================================
    // prepare the data for plotting
    // original signal comparison:
    std::vector<double> plot_original_signal(signal_length),
                        plot_inverse_signal_parallel(signal_length),
                        plot_inverse_signal_sequential(signal_length);
    // FFT signal comparison:
    // sequential FFT
    std::vector<double> plot_sequential_fft_magnitude(signal_length), plot_sequential_fft_phase(signal_length);
    // parallel FFT
    std::vector<double> plot_parallel_fft_magnitude(signal_length), plot_parallel_fft_phase(signal_length);
    // sequential Inverse FFT:
    std::vector<double> plot_sequential_inverse_fft_magnitude(signal_length),
                        plot_sequential_inverse_fft_phase(signal_length);
    // parallel Inverse FFT:
    std::vector<double> plot_parallel_inverse_fft_magnitude(signal_length),
                        plot_parallel_inverse_fft_phase(signal_length);
    // linear space for x-axis
    std::vector<double> x = matplot::linspace(0, signal_length - 1, signal_length);
    // fill the data
    for (size_t i = 0; i < signal_length; ++i) {
        // comparison of the original signal
        plot_original_signal[i] = original_signal[i].real();
        plot_inverse_signal_parallel[i] = inverse_solution[i].real();
        plot_inverse_signal_sequential[i] = inverse_solution_seq[i].real();
        // magnitude
        plot_sequential_fft_magnitude[i] = std::abs(sequential_fft_input[i]);
        plot_parallel_fft_magnitude[i] = std::abs(parallel_fft_input[i]);
        plot_parallel_inverse_fft_magnitude[i] = std::abs(parallel_inverse_fft_input[i]);
        plot_sequential_inverse_fft_magnitude[i] = std::abs(sequential_inverse_fft_input[i]);
        // phase
        plot_sequential_fft_phase[i] = std::arg(sequential_fft_input[i]);
        plot_parallel_fft_phase[i] = std::arg(parallel_fft_input[i]);
        plot_parallel_inverse_fft_phase[i] = std::arg(parallel_inverse_fft_input[i]);
        plot_sequential_inverse_fft_phase[i] = std::arg(sequential_inverse_fft_input[i]);
    }

    // create the plot for original signal comparison
    auto comparison_figure = matplot::figure();
    // title of the window
    comparison_figure->name("Plot 1D FFT Signal Comparison");
    // title of the plot
    comparison_figure->title("Original Signal vs. Inverse FFT Signal");
    // size of the window (width, height)
    comparison_figure->size(1200, 800);
    // position of the window (x, y)
    comparison_figure->x_position(0);
    comparison_figure->y_position(0);
    // plot
    comparison_figure->add_subplot(3,1,1);
    matplot::plot(x, plot_original_signal);
    matplot::title("Original Signal");
    comparison_figure->add_subplot(3,1,2);
    matplot::plot(x, plot_inverse_signal_sequential);
    matplot::title("Sequential IFFT");
    comparison_figure->add_subplot(3,1,3);
    matplot::plot(x, plot_inverse_signal_parallel);
    matplot::title("Parallel IFFT");
    // show the plot and block the execution
    comparison_figure->show();


    // create the plot for magnitude comparison
    auto magnitude_figure = matplot::figure();
    // title of the window
    magnitude_figure->name("Magnitude 1D FFT Signal Comparison");
    // title of the plot
    magnitude_figure->title("Magnitude Comparison");
    // size of the window (width, height)
    magnitude_figure->size(1300, 800);
    // position of the window (x, y)
    magnitude_figure->x_position(0);
    magnitude_figure->y_position(0);
    // plot
    magnitude_figure->add_subplot(2,2,1);
    matplot::plot(x, plot_sequential_fft_magnitude);
    matplot::title("Sequential FFT");
    magnitude_figure->add_subplot(2,2,2);
    matplot::plot(x, plot_parallel_fft_magnitude);
    matplot::title("Parallel FFT");
    magnitude_figure->add_subplot(2,2,3);
    matplot::plot(x, plot_sequential_inverse_fft_magnitude);
    matplot::title("Inverse Sequential FFT");
    magnitude_figure->add_subplot(2,2,4);
    matplot::plot(x, plot_parallel_inverse_fft_magnitude);
    matplot::title("Inverse Parallel FFT");
    // show the plot and block the execution
    magnitude_figure->show();


    // create the plot for phase comparison
    auto phase_figure = matplot::figure();
    // title of the window
    phase_figure->name("Phase 1D FFT Signal Comparison");
    // title of the plot
    phase_figure->title("Phase Comparison");
    // size of the window (width, height)
    phase_figure->size(1300, 800);
    // position of the window (x, y)
    phase_figure->x_position(0);
    phase_figure->y_position(0);
    // plot
    phase_figure->add_subplot(2,2,1);
    matplot::plot(x, plot_sequential_fft_phase);
    matplot::title("Sequential FFT");
    phase_figure->add_subplot(2,2,2);
    matplot::plot(x, plot_parallel_fft_phase);
    matplot::title("Parallel FFT");
    phase_figure->add_subplot(2,2,3);
    matplot::plot(x, plot_sequential_inverse_fft_phase);
    matplot::title("Inverse Sequential FFT");
    phase_figure->add_subplot(2,2,4);
    matplot::plot(x, plot_parallel_inverse_fft_phase);
    matplot::title("Inverse Parallel FFT");
    // show the plot and block the execution
    phase_figure->show();*/

    /* JPG COMPRESSION */
    const char* input_image_path = "src/snake_1024x1024.png";
    //prova(input_image_path);
    compress_image(input_image_path);

    const char* compressed_image_path = "compressed_image.bin";
    decompress_image(compressed_image_path);

    return 0;

    return 0;
}
