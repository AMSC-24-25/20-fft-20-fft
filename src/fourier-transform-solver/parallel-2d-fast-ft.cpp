#include <vector>
#include <complex>
#include <iostream>
#include <omp.h>
#include "fourier-transform-solver/parallel-1d-fast-ft.hpp"

class Parallel2DFastFT {
public:
    Parallel2DFastFT(std::vector<std::vector<std::complex<double>>> input_matrix)
        : data(std::move(input_matrix)) {}

    void compute() {
        const size_t rows = data.size();
        const size_t cols = data[0].size();

        // Apply 1D FFT to each row
//#pragma omp parallel for
        for (size_t i = 0; i < rows; ++i) {
            std::cout << i << std::endl;
            Parallel1DFastFT row_fft(data[i]);
            row_fft.compute();
            data[i] = row_fft.getSolution();
        }

        // Transpose the matrix
        std::vector<std::vector<std::complex<double>>> transposed(cols, std::vector<std::complex<double>>(rows));
#pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                transposed[j][i] = data[i][j];

        // Apply 1D FFT to each column (originally a row in transposed)
#pragma omp parallel for
        for (size_t i = 0; i < cols; ++i) {
            Parallel1DFastFT col_fft(transposed[i]);
            col_fft.compute();
            transposed[i] = col_fft.getSolution();
        }

        // Transpose back to original orientation
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                data[i][j] = transposed[j][i];
    }

    const std::vector<std::vector<std::complex<double>>> &getSolution() const {
        return data;
    }

private:
    std::vector<std::vector<std::complex<double>>> data;
};
