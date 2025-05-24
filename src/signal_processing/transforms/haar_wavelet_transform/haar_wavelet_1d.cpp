#include <iostream>
#include <cmath>
#include <vector>

#include "haar_wavelet_1d.hpp"

namespace sp::hwt::solver
{
    void HaarWaveletTransform1D::compute() {
        int size = input.size();

        //check if size is valid (must be even and at least 2)
        if (size < 2 || size % 2 != 0) {
            std::cerr << "Size must be even and >= 2." << std::endl;
            return;
        }

        //perform successive Haar transform steps
        while (size > 1){
            solution = haartStep(size, solution);
            size = size / 2;
        }
    }

    std::vector<double> HaarWaveletTransform1D::haartStep(int step, std::vector<double> data) {
        int size = data.size();
        std::vector<double> tmp(size);

        //copy data to temporary vector
        for(int i = 0; i < size; i++)
            tmp[i] = data[i];

        //apply the Haar transformation for the given step
        for (int i=0 ; i < step/2 ; i++){
            tmp[i] = (data[2*i + 1] + data[2*i] ) / sqrt(2);         //compute average
            tmp[step/2 + i] = (data[(2*i)] - data[(2*i + 1)]) / sqrt(2); //compute difference
        }

        return tmp; //return the transformed vector
    }
}
