#ifndef IDCT_HPP
#define IDCT_HPP

#include <vector>

namespace signal_processing::dct::algorithms {

    /**
    * Sequential Inverse Discrete Cosine Transform (IDCT) Algorithm (1D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of double numbers.
    */
    void computeIDCT1d(std::vector<double> &input);

    /**
    * Sequential Inverse Discrete Cosine Transform (IDCT) Algorithm (2D).
    *
    * The output is stored in the same input vector, which is modified in place.
    *
    * @param input The input vector of vector double numbers.
    */
    void computeIDCT2d(std::vector<std::vector<double>> &input);
}

#endif //IDCT_HPP