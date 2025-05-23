#include <iostream>
#include <vector>
#include <string>
#include "Haart1D.cpp"    //include 1D Haar transform
#include "Haart2D.cpp"    //include 2D Haar transform
#include "imgWL.cpp"      //include wavelet-based image compression

#define STB_IMAGE_IMPLEMENTATION
#include "stblib/stb_image.h"    //library to load images
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stblib/stb_image_write.h"    //library to save images

void show(std::vector<std::vector<double>> matrix){

    std::cout << "Matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl << std::endl;
}

void compare(std::vector<std::vector<double>> A, std::vector<std::vector<double>> B){
    if(A.size() != B.size() || A[0].size() != B[0].size()){
        std::cout << "incompatible\n";
        return;
    }

    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < A[0].size(); j++){
            if(A[i][j] != B[i][j]) std::cout << "row: " << i << " col: " << j << " A: " << A[i][j] << " B: " << B[i][j] <<"\n";
        }
    }
}

void proportions(std::vector<std::vector<double>> mat){

    int rows = mat.size();
    int cols = mat[0].size();
    int nonzero = 0;

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++) if(mat[i][j] != 0) nonzero++;
    
    std::cout << "total elements: " << rows*cols << ", nonzero elemts: " << nonzero << ", proportion: " << (rows*cols)/nonzero << std::endl;
}

int main(){

    //==================================================1D Haar wavelet transform example==================================================

    /*std::vector<double> input = { 1.0, 2.0, 3.0, 1.0, 2.0, 3.0, 4.0, 0.0};    //example input for 1D transform
    //std::vector<double> input = { 9.0, 7.0, 3.0, 5.0};                        //test vector

    std::cout << "1D Haar wavelet transform example: " << std::endl;
    std::cout << "Input vector: [";
    for(int i = 0; i < input.size(); i++)
        std::cout << input[i] << "  ";
    std::cout << "]" << std::endl;

    HaarWaveletTransform1D waveletT1D(input);  //create object for 1D transform
    waveletT1D.compute();                     //compute the transform
    std::vector<double> vecSolution = waveletT1D.getSolution(); //get the solution

    std::cout << "Solution vector: [";
    for(int i = 0; i < input.size(); i++)
        std::cout << vecSolution[i] << "  ";
    std::cout << "]" << std::endl << std::endl;*/


    //==================================================2D Haar wavelet transform example==================================================

    /*alternative test matrix
    std::vector<std::vector<double>> matrix = {{0, 1, 1, 0}, {1, 0, 0,1}, {1, 0, 0,1}, {0, 1, 1, 0}};*/
    
    // Example 2D input matrix (8x8 square)
    /*std::vector<std::vector<double>> matrix = {{64, 2, 3, 61, 60, 6, 7, 57},  
                                                {9, 55, 54, 12, 13, 51, 50, 16}, 
                                                {17, 47, 46, 20, 21, 43, 42, 24},
                                                {40, 26, 27, 37, 36, 30, 31, 33},
                                                {32, 34, 35, 29, 28, 38, 39, 25},
                                                {41, 23, 22, 44, 45, 19, 18, 48},
                                                {49, 15, 14, 52, 53, 11, 10, 56},
                                                {8, 58, 59, 5, 4, 62, 63, 1}};
    
    

    std::cout << "2D Haar wavelet transform example: " << std::endl;
    std::cout << "Input matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl;

    HaarWaveletTransform2D waveletT2D(matrix); //create object for 2D transform
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
    std::cout << std::endl << std::endl;*/

    //=======================================2D Haar wavelet transform for image compression example=======================================

    int w, h, channels;
    std::string name = "dog";
    std::string img_path= name + ".png";
    unsigned char* image_data = stbi_load(img_path.c_str(), &w, &h, &channels, 0); //load image

    if(!image_data){
        std::cout << "could not load image " << img_path << std::endl;
        return 1; //if loading fails, terminate program
    }

    std::vector<std::vector<double>> image(h, std::vector<double>(w)); //vector to hold the image
    std::cout << "Image compression through Haar wavelet transform example:" << std::endl;

    //copy image pixels to double vector
    for(int i = 0; i<h; i++)
        for(int j = 0; j<w; j++)
            image[i][j]= static_cast<double>(image_data[(i*w+j)*channels]);

    std::cout << "Image: " << img_path << " loaded" << std::endl;

    ImgWLComp imgWL;        //create object for image compression

    std::vector<std::vector<double>> solution = imgWL.compress(image);       //compress the image;

    //prepare vector to save compressed image
    std::vector<unsigned char> outputImage(h*w);
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(solution[i][j]);
    
    //save compressed image
    std::string compressed_path = "compressed-" + img_path;
    if (stbi_write_png(compressed_path.c_str(), w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to compressed-face.png" << std::endl;
        return 1;
    }

    std::string binary_path = "compressed-" + name + ".bin";
    imgWL.save_as_binary(solution, binary_path);

    std::cout << "Image compressed and saved as " << binary_path << std::endl;
    std::cout << "Reconstructing image" << std::endl;

    std::vector<std::vector<double>> reconstructed = imgWL.load_img_from_binary(binary_path);
    reconstructed = imgWL.reconstruct(reconstructed);

    //prepare vector to save decompressed image
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(reconstructed[i][j]);
    
    //save decompressed image
    std::string reconstructed_path = "reconstructed-" + name+ ".png";
    if (stbi_write_png(reconstructed_path.c_str(), w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to " << reconstructed_path << std::endl;
        return 1;
    }

    std::cout << "Image reconstructed and saved as reconstructed" << reconstructed_path << std::endl;

    return 0;
}
