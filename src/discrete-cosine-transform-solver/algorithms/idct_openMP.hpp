#ifndef IDCT_OPENMP_HPP
#define IDCT_OPENMP_HPP

#include <iostream>
#include <vector>
#include <type_traits>
#include <cmath>

namespace dct::algorithms {
    /**
    * Parallel Inverse Discrete Cosine Transform (IDCT) Algorithm (1D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of double numbers.
    */
    void compute_idct_1d_openMP(std::vector<double> &input);

    /**
    * Parallel Inverse Discrete Cosine Transform (IDCT) Algorithm (2D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of vector double numbers.
    */
    void compute_idct_2d_openMP(std::vector<std::vector<double>> &input);
}

#endif