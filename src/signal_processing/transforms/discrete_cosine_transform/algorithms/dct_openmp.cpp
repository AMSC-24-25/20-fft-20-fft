#include <cmath>

#include "transforms/discrete_cosine_transform/algorithms/dct_openmp.hpp"

namespace signal_processing::dct::algorithms{
    void computeDCT1dOpenMP(std::vector<double> &input){
        const size_t N = input.size();
        const std::vector<double> data = input;

        #pragma omp parallel for
        for (int k=0; k<N; ++k) {
            double sum = 0.0;
            for (int n=0; n<N; ++n) {
                sum += data[n] * std::cos((M_PI*(2*n+1)*k)/(2.0*N));
            }

            if (k==0) {
                input[k] = std::sqrt(1.0/N) * sum; // alpha = sqrt(1/N)
            }
            else {
                input[k] = std::sqrt(2.0/N) * sum; // alpha = sqrt(2/N)
            }
        }
    }

    void computeDCT2dOpenMP(std::vector<std::vector<double>> &input){
        const size_t rows = input.size();
        const size_t cols = input[0].size();

        // Apply 1D DCT to each row
        #pragma omp parallel for
        for (size_t i = 0; i < rows; ++i) {
            computeDCT1dOpenMP(input[i]);
        }

        // Transpose the matrix
        std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
        #pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                transposed[j][i] = input[i][j];


        // Apply 1D DCT to each column (originally a row in transposed)
        #pragma omp parallel for
        for (size_t i = 0; i < cols; ++i) {
            // NB: not rows because rows and cols refer to the original image and
            //     we do not update them after the transpose
            computeDCT1dOpenMP(transposed[i]);
        }

        // Transpose back to original orientation
        #pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                input[i][j] = transposed[j][i];
    }
}
