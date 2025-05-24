#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "compression/image_compression_haar_wavelet/image_compression_haar_wavelet.hpp"

namespace sp::hwt {
    // Compresses the input image matrix using Haar wavelet transform and thresholding
    std::vector<std::vector<double>> ImgWLComp::compress(
        const std::vector<std::vector<double>>& input,
        const double threshold
    ){
        const int rows = input.size();
        const int cols = input[0].size();
        std::vector<std::vector<double>> compressed = input;

        transformRows(compressed);
        transpose(compressed);
        transformRows(compressed);
        transpose(compressed);

        for(int i = 0; i < rows; ++i)
            for(int j = 0; j < cols; ++j)
                if(std::abs(compressed[i][j]) <= threshold) compressed[i][j] = 0;

        return compressed;
    }


    // Reconstructs the original image matrix from its compressed (transformed) version
    std::vector<std::vector<double>> ImgWLComp::reconstruct(const std::vector<std::vector<double>>& input){

        std::vector<std::vector<double>> decompressed = input;

        transpose(decompressed);
        reconstructRows(decompressed);
        transpose(decompressed);
        reconstructRows(decompressed);

        return decompressed;
    }


    // Saves the transformed image matrix to a binary file with basic RLE compression
    void ImgWLComp::save_as_binary(std::vector<std::vector<double>> img_matrix, const std::string& path){
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

        /*Since, after applying Haar wavelet transform, the coefficient are in the range [-255,255], this for cicle is used to bring them in the range
          [-127,127], so that the elements can be saved as int_8t and save memory*/
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                img_matrix[i][j] /= 2;
            }
        }

        /*normal rle*/

        for (int r=0; r<rows; ++r) {
            for (int c=0; c<cols; ++c) {

                int8_t value = static_cast<int8_t>(img_matrix[r][c]);
                file.write(reinterpret_cast<char*>(&value), sizeof(value));

                if(value == 0){
                    while(img_matrix[r][c] == 0 && c < cols - 1) c++;
                    if(c != cols -1) c--;
                    file.write(reinterpret_cast<char*>(&c), sizeof(c));
                }
            }
        }

        file.close();
    }


    // Loads a matrix from a binary file and reconstructs the image data with scaling
    std::vector<std::vector<double>> ImgWLComp::load_img_from_binary(const std::string& compressed_image_path){

        // Open the file in binary mode to read
        std::ifstream file(compressed_image_path, std::ios::binary);

        if (!file) {
            std::cerr << "Errore nell'apertura del file!" << std::endl;
            throw std::runtime_error("Error opening binary file!");
        }

        // Read the matrix size
        size_t rows, cols, zigzagdim;
        file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
        file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

        // Read matrix elements and store them in img_matrix
        /*normal rle*/
        std::vector<std::vector<double>> img_matrix(rows, std::vector<double>(cols, 0));

        for (int r=0; r < rows; ++r) {
            for (int c=0; c < cols; ++c) {

                int8_t value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                img_matrix[r][c] = static_cast<double>(value);

                if(value == 0){
                    file.read(reinterpret_cast<char*>(&c), sizeof(c));
                }
            }
        }

        file.close();

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                img_matrix[i][j] *= 2;
            }
        }

        return img_matrix;
    }


    // Transposes a given matrix (rows become columns and vice versa)
    void ImgWLComp::transpose(std::vector<std::vector<double>>& mat){
        const int n = mat.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                // Using std::swap because it is more efficient and cleaner
                std::swap(mat[i][j], mat[j][i]);
            }
        }
    }


    // Applies inverse Haar wavelet transform to each row of the matrix
    void ImgWLComp::reconstructRows(std::vector<std::vector<double>>& input){
        const int rows = input.size();
        for(int i = 0, size = 2; i < rows; ++i, size = 2){
            while(size <= rows){
                input[i] = invertStep(size, input[i]);
                size *= 2;
            }
        }
    }


    // Performs one step of inverse Haar wavelet transform on a single row
    std::vector<double> ImgWLComp::invertStep(const int size, const std::vector<double>& data){
        const int half = size / 2;
        std::vector<double> tmp = data;

        for(int i = 0, i2 = 0; i < half; ++i, i2 += 2){
            tmp[i2] = data[half + i] + data[i];
            tmp[i2 + 1] = 2*data[i] - tmp[i2];
        }

        return tmp;
    }


    // Applies forward Haar wavelet transform to each row of the matrix
    void ImgWLComp::transformRows(std::vector<std::vector<double>>& input){
        const int rows = input.size();

        for(int i = 0; i < rows; ++i){
            int size = input[0].size();

            while(size > 1){
                input[i] = haartStep(size, input[i]);
                size /= 2;
            }
        }
    }


    // Performs one step of forward Haar wavelet transform on a single row
    std::vector<double> ImgWLComp::haartStep(const int step, const std::vector<double>& data){
        const int half_step = step / 2;
        std::vector<double> tmp = data;

        for (int i=0 ; i < half_step ; ++i){
            const int i2 = 2*i;
            tmp[i] = (data[i2 + 1] + data[i2] ) / 2;
            tmp[half_step + i] = (data[i2] - data[(i2 + 1)]) / 2;
        }

        return tmp;
    }
}
