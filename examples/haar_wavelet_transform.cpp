#include <iostream>
#include <vector>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "signal_processing/compression/image_compression_haar_wavelet/image_compression_haar_wavelet.hpp"
#include "signal_processing/transforms/haar_wavelet_transform/haar_wavelet_1d.hpp"
#include "signal_processing/transforms/haar_wavelet_transform/haar_wavelet_2d.hpp"

int main(){

    //==================================================1D Haar wavelet transform example==================================================

    std::vector<double> input = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 4.0, 0.0}; //example input for 1D transform

    std::cout << "1D Haar wavelet transform example: " << std::endl;
    std::cout << "Input vector: [";
    for(int i = 0; i < input.size(); i++)
        std::cout << input[i] << "  ";
    std::cout << "]" << std::endl;

    //create object for 1D transform
    signal_processing::hwt::solver::HaarWaveletTransform1D waveletT1D(input);
    //compute the transform
    waveletT1D.compute();
    //get the solution
    std::vector<double> vecSolution = waveletT1D.getSolution();

    std::cout << "Solution vector: [";
    for(int i = 0; i < input.size(); i++)
        std::cout << vecSolution[i] << "  ";
    std::cout << "]" << std::endl << std::endl;


    //==================================================2D Haar wavelet transform example==================================================

    // Example 2D input matrix (8x8 square)
    std::vector<std::vector<double>> matrix = {{64, 2, 3, 61, 60, 6, 7, 57},
                                                {9, 55, 54, 12, 13, 51, 50, 16},
                                                {17, 47, 46, 20, 21, 43, 42, 24},
                                                {40, 26, 27, 37, 36, 30, 31, 33},
                                                {32, 34, 35, 29, 28, 38, 39, 25},
                                                {41, 23, 22, 44, 45, 19, 18, 48},
                                                {49, 15, 14, 52, 53, 11, 10, 56},
                                                {8, 58, 59, 5, 4, 62, 63, 1}};

    /*alternative test matrix
    std::vector<std::vector<double>> matrix = {{0, 1, 1, 0}, {1, 0, 0,1}, {1, 0, 0,1}, {0, 1, 1, 0}};*/

    std::cout << "2D Haar wavelet transform example: " << std::endl;
    std::cout << "Input matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl;

    signal_processing::hwt::solver::HaarWaveletTransform2D waveletT2D(matrix); //create object for 2D transform
    waveletT2D.compute();                      //compute the transform
    std::vector<std::vector<double>> matSolution = waveletT2D.getSolution(); //get the solution
    std::vector<std::vector<double>> Hn = waveletT2D.getHnMatrix();          //get Hn matrix

    std::cout << "Corresponding Hn matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << Hn[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl << std::endl;

    std::cout << "Solution matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << matSolution[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl << std::endl;

    //=======================================2D Haar wavelet transform for image compression example=======================================

    int w, h, channels;
    unsigned char* image_data = stbi_load("cat-original.png", &w, &h, &channels, 0); //load image

    if(!image_data){
        std::cout << "could not load image cat-original" << std::endl;
        return 1; //if loading fails, terminate program
    }

    std::vector<std::vector<double>> image(h, std::vector<double>(w)); //vector to hold the image
    std::cout << "Image compression through Haar wavelet transform example:" << std::endl;

    //copy image pixels to double vector
    for(int i = 0; i<h; i++)
        for(int j = 0; j<w; j++)
            image[i][j]= static_cast<double>(image_data[(i*w+j)*channels]);

    std::cout << "Image: 'cat-original.png' loaded" << std::endl;

    signal_processing::compression::hwt::ImgWLComp imgWL(image); //create object for image compression
    imgWL.compress();       //compress the image

    std::vector<std::vector<double>> solution(h, std::vector<double>(w));
    solution = imgWL.getCompressed(); //get compressed image

    //prepare vector to save compressed image
    std::vector<unsigned char> outputImage(h*w);
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(solution[i][j]);

    //save compressed image
    if (stbi_write_png("compressed-cat.png", w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to compressed-cat.png" << std::endl;
        return 1;
    }

    std::cout << "Image compressed and saved as compressed-cat.png" << std::endl;
    std::cout << "Reconstructing image" << std::endl;

    imgWL.reconstruct();    //decompress the image
    solution = imgWL.getDecompressed(); //get decompressed image

    //prepare vector to save decompressed image
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(solution[i][j]);

    //save decompressed image
    if (stbi_write_png("reconstructed-cat.png", w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to reconstructed-cat.png" << std::endl;
        return 1;
    }

    std::cout << "Image reconstructed and saved as reconstructed-cat.png" << std::endl;

    return 0;
}
