#ifndef DCT_OPENMP_HPP
#define DCT_OPENMP_HPP

#include <iostream>
#include <vector>
#include <type_traits>
#include <cmath>

namespace dct::algorithms {

    /**
    * Parallel Discrete Cosine Transform (DCT) Algorithm (1D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of double numbers.
    */
    void compute_dct_1d_openMP(std::vector<double> &input);

    /**
    * Parallel Discrete Cosine Transform (DCT) Algorithm (2D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of vector double numbers.
    */
    void compute_dct_2d_openMP(std::vector<std::vector<double>> &input);
}

#endif