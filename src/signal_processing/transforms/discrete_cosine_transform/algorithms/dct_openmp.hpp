#ifndef DCT_OPENMP_HPP
#define DCT_OPENMP_HPP

#include <vector>

namespace sp::dct::algo {

    /**
    * Parallel Discrete Cosine Transform (DCT) Algorithm (1D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of double numbers.
    */
    void computeDCT1dOpenMP(std::vector<double> &input);

    /**
    * Parallel Discrete Cosine Transform (DCT) Algorithm (2D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of vector double numbers.
    */
    void computeDCT2dOpenMP(std::vector<std::vector<double>> &input);
}

#endif