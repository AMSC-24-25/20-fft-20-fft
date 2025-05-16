#include <iostream>
#include <vector>
#include <cmath>

#include "image_compression_haar_wavelet.hpp"

namespace signal_processing::compression::hwt
{
    void ImgWLComp::compress() {
        transformRows();            //apply transform to rows
        transpose(compressed);      //transpose the matrix
        transformRows();            //apply transform to columns (after transpose)
        transpose(compressed);      //transpose back

        //zero out small coefficients (thresholding)
        for(int i = 0; i < input.size(); i++)
            for(int j = 0; j < input[0].size(); j++)
                if(std::abs(compressed[i][j]) <= 5) compressed[i][j] = 0;
    }

    void ImgWLComp::reconstruct() {
        decompressed = compressed;

        transpose(decompressed);    //transpose
        reconstructRows();          //reconstruct rows
        transpose(decompressed);    //transpose back
        reconstructRows();          //reconstruct columns
    }

    void ImgWLComp::transpose(std::vector<std::vector<double>>& mat) {
        std::vector<std::vector<double>> temp(mat.size(), std::vector<double>(mat[0].size()));
        int r = mat.size(), c = mat[0].size();
        for(int i = 0; i < r; i++)
            for(int j = 0; j < c; j++)
                temp[i][j] = mat[j][i];

        mat = temp;
    }

    void ImgWLComp::reconstructRows() {
        int rows = input.size();
        int size;
        int step = 1;

        for(int i = 0; i < rows; i++){

            size = 2;
            while(size <= rows){

                decompressed[i] = invertStep(size, decompressed[i]);
                size = size * 2;
            }
        }
    }

    std::vector<double> ImgWLComp::invertStep(int size, std::vector<double> data) {
        std::vector<double> temp = data;
        int half = size/2;

        for(int i = 0; i < half; i++){

            temp[2*i] = data[half + i] + data[i];     //reconstruct the first element
            temp[2*i + 1] = 2*data[i] - temp[2*i];    //reconstruct the second element
        }

        return temp;
    }

    void ImgWLComp::transformRows() {
        int rows = input.size();
        int size;

        for(int i = 0; i < rows; i++){

            size = input[0].size();

            while(size > 1){

                compressed[i] = haartStep(size, compressed[i]);
                size = size / 2;
            }
        }
    }

    std::vector<double> ImgWLComp::haartStep(int step, std::vector<double> data) {
        int size = data.size();
        std::vector<double> tmp(size);

        //copy data to temporary array
        for(int i = 0; i < size; i++)
            tmp[i] = data[i];

        //compute averages and differences
        for (int i=0 ; i < step/2 ; i++){
            tmp[i] = (data[2*i + 1] + data[2*i] ) / 2;
            tmp[step/2 + i] = (data[(2*i)] - data[(2*i + 1)]) / 2;
        }

        return tmp;
    }

    void ImgWLComp::showV(std::vector<double> vec) {
        std::cout << "vector= [ ";

        for(int i = 0; i < vec.size(); i++)
            std::cout << vec[i] << " ";

        std::cout << "]\n";
    }

    void ImgWLComp::showM(std::vector<std::vector<double>> mat) {
        std::cout << "Matrix: " << std::endl << std::endl << "\t";
        for(int i = 0; i < mat.size(); i++){
            for(int j = 0; j < mat[0].size(); j++){
                std::cout << mat[i][j] << "\t";
            }
            std::cout << std::endl << "\t";
        }
        std::cout << std::endl << std::endl;
    }

}
