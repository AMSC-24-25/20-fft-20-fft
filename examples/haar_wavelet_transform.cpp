#include <chrono>
#include <iostream>
#include <vector>
#include <string>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "signal_processing/signal_processing.hpp"

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

    //create object for 1D transform
    sp::hwt::solver::HaarWaveletTransform1D waveletT1D(input);
    //compute the transform
    waveletT1D.compute();
    //get the solution
    std::vector<double> vecSolution = waveletT1D.getSolution();

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



    std::cout << "2D Haar wavelet transform example: " << std::endl;
    std::cout << "Input matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl;

    sp::hwt::solver::HaarWaveletTransform2D waveletT2D(matrix); //create object for 2D transform
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

    // ask user for threshold
    std::cout << "Enter threshold for image compression (0.0 - 5.0): ";
    std::string threshold;
    std::getline(std::cin, threshold);
    if (threshold.empty()) {
        std::cout << "No threshold provided, using default value of 0.5" << std::endl;
        threshold = "0.5"; //default value
    }

    int w, h, channels;
    const std::string path = "examples/resources/";
    const std::string name = "dog-bw";
    const std::string ext = ".png";
    const std::string img_path= path + name + ext;
    // load image
    const unsigned char* image_data = stbi_load(img_path.c_str(), &w, &h, &channels, 0);

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

    sp::hwt::ImgWLComp imgWL; //create object for image compression

    // get compressed image
    // measure time for compression
    const auto start = std::chrono::high_resolution_clock::now();
    const std::vector<std::vector<double>> solution = imgWL.compress(image, std::stod(threshold));
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> duration = end - start;
    printf("Image compressed in %.2f seconds\n", duration.count());

    //prepare vector to save compressed image
    std::vector<unsigned char> outputImage(h*w);
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            outputImage[j + i*w] = static_cast<unsigned char>(solution[i][j]);

    //save compressed image
    const std::string compressed_path = "examples/output/compressed-" + name + "-threshold-" + threshold + ext;
    if (stbi_write_png(compressed_path.c_str(), w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to " << compressed_path << std::endl;
        return 1;
    }

    // TODO: bug with threshold 1
    /*
    std::string binary_path = "examples/output/compressed-" + name + ".bin";
    imgWL.save_as_binary(solution, binary_path);

    std::cout << "Image compressed and saved as " << binary_path << std::endl;
    std::cout << "Reconstructing image" << std::endl;

    std::vector<std::vector<double>> reconstructed = imgWL.load_img_from_binary(binary_path);
    reconstructed = imgWL.reconstruct(reconstructed);
    */

    // measure time for reconstruction
    printf("Reconstructing image...\n");
    const auto start_reconstruct = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<double>> reconstructed = imgWL.reconstruct(solution);
    const auto end_reconstruct = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> duration_reconstruct = end_reconstruct - start_reconstruct;
    printf("Image reconstructed in %.2f seconds\n", duration_reconstruct.count());

    //prepare vector to save decompressed image
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++){
            //clamp values to [0, 255] range
            reconstructed[i][j] = reconstructed[i][j] > 255.0 ? 255.0 : (reconstructed[i][j] < 0.0 ? 0.0 : reconstructed[i][j]);
            outputImage[j + i*w] = static_cast<unsigned char>(reconstructed[i][j]);
        }

    //save decompressed image
    const std::string reconstructed_path = "examples/output/reconstructed-" + name + "-threshold-" + threshold + ext;
    if (stbi_write_png(reconstructed_path.c_str(), w, h, channels, outputImage.data(), w) == 0) {
        std::cerr << "Error: Could not save image to " << reconstructed_path << std::endl;
        return 1;
    }

    std::cout << "Image reconstructed and saved as " << reconstructed_path << std::endl;

    return 0;
}
