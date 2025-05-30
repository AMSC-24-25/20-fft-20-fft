#include <cmath>

#include "transforms/discrete_cosine_transform/algorithms/idct_openmp.hpp"

namespace sp::dct::algo {
    void computeIDCT1dOpenMP(std::vector<double> &input){
        const size_t N = input.size();
        std::vector<double> dct_coefficients = input;

        #pragma omp parallel for
        for (int n=0; n<N; ++n) {
            double sum = 0.0;
            for (int k=0; k<N; ++k) {
                if (k==0) {
                    // because the first element of dct has a different scaled factor
                    sum += std::sqrt(1.0/N) * dct_coefficients[k];
                }
                else {
                    sum += std::sqrt(2.0/N) * dct_coefficients[k] * std::cos((M_PI * (2*n+1)*k)/(2.0*N));
                }
            }
            input[n] = sum;
        }
    }

    void computeIDCT2dOpenMP(std::vector<std::vector<double>> &input){
        const size_t rows = input.size();
        const size_t cols = input[0].size();

        // Apply 1D IDCT to each row
        #pragma omp parallel for
        for (size_t i = 0; i < rows; ++i) {
            computeIDCT1dOpenMP(input[i]);
        }

        // Transpose the matrix
        std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
        #pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                transposed[j][i] = input[i][j];


        // Apply 1D IDCT to each column (originally a row in transposed)
        #pragma omp parallel for
        for (size_t i = 0; i < cols; ++i) {
            // NB: not rows because rows and cols refer to the original image and
            //     we do not update them after the transpose
            computeIDCT1dOpenMP(transposed[i]);
        }

        // Transpose back to original orientation
        #pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                input[i][j] = transposed[j][i];
    }
}
