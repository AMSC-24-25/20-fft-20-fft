#include "discrete_cosine_transform/algorithms/idct.hpp"

namespace dct::algorithms {
    /* Function that computes the inverse 1D Discrete Cosine Transform (IDCT) of a vector.
     * Reconstructs the original signal from its DCT coefficients, modifying it in-place.
     * @input: reference to the DCT coefficient vector to invert
     */
    void compute_idct_1d(std::vector<double> &input){
        const size_t N = input.size();
        std::vector<double> dct_coefficients = input;

        for (int n=0; n<N; ++n) {
            double sum = 0.0;
            for (int k=0; k<N; ++k) {
                if (k==0) {
                    sum += std::sqrt(1.0/N) * dct_coefficients[k]; //because the first element of dct has a different scaled factor
                }
                else {
                    sum += std::sqrt(2.0/N) * dct_coefficients[k] * std::cos((M_PI * (2*n+1)*k)/(2.0*N));
                }
            }
            input[n] = sum;
        }
    };

    void compute_idct_2d(std::vector<std::vector<double>> &input){
        const size_t rows = input.size();
        const size_t cols = input[0].size();

        // Apply 1D IDCT to each row
        for (size_t i = 0; i < rows; ++i) {
            compute_idct_1d(input[i]);
        }

        // Transpose the matrix
        std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                transposed[j][i] = input[i][j];


        // Apply 1D IDCT to each column (originally a row in transposed)
        for (size_t i = 0; i < cols; ++i) { //NB: not rows because rows and cols refer to the original image and we do not update them after the transpose
            compute_idct_1d(transposed[i]);
        }

        // Transpose back to original orientation
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                input[i][j] = transposed[j][i];
    };
}