#ifndef IDCT_OPENMP_HPP
#define IDCT_OPENMP_HPP

#include <vector>

namespace sp::dct::algo {
    /**
    * Parallel Inverse Discrete Cosine Transform (IDCT) Algorithm (1D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of double numbers.
    */
    void computeIDCT1dOpenMP(std::vector<double> &input);

    /**
    * Parallel Inverse Discrete Cosine Transform (IDCT) Algorithm (2D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of vector double numbers.
    */
    void computeIDCT2dOpenMP(std::vector<std::vector<double>> &input);
}

#endif