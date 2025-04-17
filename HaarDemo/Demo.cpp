#include <iostream>
#include <vector>
#include <string>
#include "Haart1D.cpp"
#include "Haart2D.cpp"
#include "imgWL.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stblib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stblib/stb_image_write.h"


int main(){

    //==================================================1D Haar wavelet transform example==================================================

    std::vector<double> input = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 4.0, 0.0};;

    std::cout << "1D Haar wavelet transform example: " << std::endl;
    std::cout << "Input vector: [";
    for(int i = 0; i < input.size(); i++)
        std::cout << input[i] << "  ";
    std::cout << "]" << std::endl;

    HaarWaveletTransform1D waveletT1D(input);
    waveletT1D.compute();
    std::vector<double> vecSolution = waveletT1D.getSolution();

    std::cout << "Solution vector: [";
    for(int i = 0; i < input.size(); i++)
        std::cout << vecSolution[i] << "  ";
    std::cout << "]" << std::endl << std::endl;

    //input.~vector()                           optional if memory is needed
    //vecSolution.~vector()                     optional if memory is needed
    //wavwlet1D.~HaarWaveletTransform1D()       optional if memory is needed

    //==================================================2D Haar wavelet transform example==================================================

    /*std::vector<std::vector<double>> matrix = {{64, 2, 3, 61, 60, 6, 7, 57},  
                                                {9, 55, 54, 12, 13, 51, 50, 16}, 
                                                {17, 47, 46, 20, 21, 43, 42, 24},
                                                {40, 26, 27, 37, 36, 30, 31, 33},
                                                {32, 34, 35, 29, 28, 38, 39, 25},
                                                {41, 23, 22, 44, 45, 19, 18, 48},
                                                {49, 15, 14, 52, 53, 11, 10, 56},
                                                {8, 58, 59, 5, 4, 62, 63, 1}};*/

    std::vector<std::vector<double>> matrix = {{0, 1, 1, 0 },{1, 0, 0, 1 },{1, 0, 0, 1 },{0, 1, 1, 0 }};

    std::cout << "2D Haar wavelet transform example: " << std::endl;
    std::cout << "Input matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl;

    HaarWaveletTransform2D waveletT2D(matrix);
    waveletT2D.compute();
    std::vector<std::vector<double>> matSolution = waveletT2D.getSolution();

    std::cout << "Solution matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << matSolution[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl << std::endl;

    //matrix.~vector<std::vector<double>>()             optional if memory is needed
    //waveletT2D.~HaarWaveletTransform2D()              optional if memory is needed
    //matSolution.~vector<std::vector<double>>()        optional if memory is needed

    //=======================================2D Haar wavelet transform for image compression example=======================================

    int w, h, channels; 
    unsigned char* image_data = stbi_load("cat-original.png", &w, &h, &channels, 0);

    if(!image_data){
        std::cout << "could not load image cat-original" << std::endl;
        return 1;
    }

    std::vector<std::vector<double>> image(h, std::vector<double>(w));
    std::cout << "Image compression trough Haar wavelet transform example:" << std::endl;

    for(int i = 0; i<h; i++)
        for(int j = 0; j<w; j++)
            image[i][j]= static_cast<double>(image_data[(i*w+j)*channels]);

    std::cout << "Image: 'cat-original.png' loaded" << std::endl;

    ImgWLComp imgWL(image);
    imgWL.compress();

    std::vector<std::vector<double>> solution(h, std::vector<double>(w));
    solution = imgWL.getCompressed();

    std::vector<unsigned char> outputImage(h*w);
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(solution[i][j]);
    
    if (stbi_write_png("compressed-cat.png", w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to compressed-cat.png" << std::endl;
        return 1;
    }

    std::cout << "Image compressed and saved as compressed-cat.png" << std::endl;
    std::cout << "Reconstructing image" << std::endl;

    imgWL.reconstruct();
    solution = imgWL.getDecompressed();

    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(solution[i][j]);
    
    if (stbi_write_png("reconstructed-cat.png", w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to reconstructe-cat.png" << std::endl;
        return 1;
    }

    std::cout << "Image reconstructed and saved as reconstructed-cat.png" << std::endl;

    return 0;
}