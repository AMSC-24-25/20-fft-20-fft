#include <iostream>
#include <vector>

#include "signal_processing/signal_processing.hpp"

/**
 * Environment variable name for the file path for the configuration file.
 */
#define ENV_FILE_PATH "CONFIG_FILE_PATH_FFT"

void print_vector(const std::vector<double>& vec) {
    for (int i=0; i<vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

void print_matrix(const std::vector<std::vector<double>>& mat) {
    for (int i=0; i<mat.size(); i++) {
        for (int j=0; j<mat[0].size(); j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ZIGZAG TEST
void zigzag_demo(){
    std::cout<<"\n================================================= ZIGZAG demo ================================================="<<std::endl;

    std::vector<std::vector<double>> matrix = {
        {1,1,1,4,5,6,7,0},
        {1,1,1,12,13,14,0,16},
        {1,18,19,20,21,0,23,24},
        {25,26,27,28,0,30,31,32},
        {33,34,35,0,37,38,39,40},
        {41,42,0,44,45,46,47,62},
        {49,0,51,52,53,54,62,56},
        {0,58,59,60,61,62,63,64}
    };

    std::cout<<"\nInput matrix to ZigZag scan: "<<std::endl;
    print_matrix(matrix);

    std::vector<double> zz = sp::utils::zigzag::ZigZagScan::scan(matrix);
    std::cout<<"\nOutput ZigZag scan: "<<std::endl;
    print_vector(zz);

    std::cout<<"\nOutput inverse ZigZag scan: "<<std::endl;
    std::vector<std::vector<double>> reverse_zz = sp::utils::zigzag::ZigZagScan::inverse_scan(zz,8,8);
    print_matrix(reverse_zz);
}

// RLE_COMPRESSOR TEST
void rle_compression_demo(){
    std::cout<<"\n================================================= RLE COMPRESSION demo ================================================="<<std::endl;

    std::vector<double> vector = {2, 0, 0, 0, 0, 34, 17, 45, 45, 45, 12, 7, 9, 9, 9, 9, 9, 9, 5};
    std::cout << "\nIutput vector for RLE compression: "<<std::endl;
    print_vector(vector);

    std::vector<std::pair<int, int>> rle_vector = sp::utils::rle::RLECompressor::compress(vector);
    std::cout << "\nOutput RLE compression: "<<std::endl;
    for(int i=0; i<rle_vector.size(); ++i){
        std::cout<<rle_vector[i].first<<" "<<rle_vector[i].second<<std::endl;
    }

    std::vector<double> rle_reverse = sp::utils::rle::RLECompressor::decompress(rle_vector);
    std::cout << "\nOutput RLE compression: "<<std::endl;
    print_vector(rle_reverse);
}

// DCT TEST
void simple_dct_demo(){
    std::cout<<"\n================================================= Simple DCT example ================================================="<<std::endl;

    std::vector<double> vect = {1,2,3,4};
    std::cout<<"Input vector: ";
    print_vector(vect);

    // Compute the direct DCT in place and in parallel mode
    sp::dct::solver::DiscreteCosineTransform dct_solver= sp::dct::solver::DiscreteCosineTransform();
    sp::dct::solver::ComputationMode cm = sp::dct::solver::ComputationMode::OPENMP;
    dct_solver.compute(vect, cm);
    std::cout<<"Output DCT (parallel mode): ";
    print_vector(vect);

    // Compute the inverse DCT in place and in parallel mode
    sp::dct::solver::InverseDiscreteCosineTransform idct_solver= sp::dct::solver::InverseDiscreteCosineTransform();
    dct_solver.compute(vect, cm);
    std::cout<<"Output IDCT (parallel mode): ";
    print_vector(vect);
}

void jpeg_compression_demo(){
    std::cout<<"\n================================================= JPEG COMPRESSION demo ================================================="<<std::endl;

    // Create an Image object contining the image from image_path and save it as png
    const char* image_path = "examples/resources/dog-bw.png";
    sp::jpeg::Image image = sp::jpeg::Image(image_path);
    std::string path = "examples/output/dct/image.png";
    // check if output directory exists, if not create it
    if (!std::filesystem::exists("examples/output/dct")) {
        std::filesystem::create_directories("examples/output/dct");
    }
    image.save_as_png(path);

    // Compress image and save it first in a binay file and then in a compressed binary file (ZIGZAG+RLE)
    sp::jpeg::CompressedImage compressed = image.compress();
    path = "examples/output/dct/binary.bin";
    compressed.save_as_binary(path);
    path = "examples/output/dct/compressed_binary.bin";
    compressed.save_as_compressed_binary(path);

    // Decompress the compressed image and save the result as a png
    sp::jpeg::Image decompressed = compressed.decompress();
    path = "examples/output/dct/decompressed.png";
    decompressed.save_as_png(path);
}

int main() {
    //simple_dct_demo();
    zigzag_demo();
    rle_compression_demo();
    jpeg_compression_demo();

    return 0;
}

