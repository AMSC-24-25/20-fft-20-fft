#include "discrete_cosine_transform.hpp"

namespace dct::solver {
    /**
     * Internal method to compute the Discrete Cosine Transform in sequential mode.
     *
     * This method modifies the input vector in place.
     *
     * @param input The input vector to be transformed.
     */
    void DiscreteCosineTransform::computeSequential(std::vector<double> &input){
        return dct::algorithms::compute_dct_1d(input);
    };

    /**
     * Internal method to compute the Discrete Cosine Transform in parallel mode using OpenMP.
     *
     * This method modifies the input vector in place.
     *
     * @param input The input vector to be transformed.
     */
    void DiscreteCosineTransform::computeOpenMP(std::vector<double> &input){
        return dct::algorithms::compute_dct_1d_openMP(input);
    };

    /**
     * Internal method to compute the Discrete Cosine Transform in sequential mode.
     *
     * This method modifies the input matrix in place.
     *
     * @param input The input matrix to be transformed.
     */
    void DiscreteCosineTransform::computeSequential(std::vector<std::vector<double>> &input){
        return dct::algorithms::compute_dct_2d(input);
    };

    /**
     * Internal method to compute the Discrete Cosine Transform in parallel mode using OpenMP.
     *
     * This method modifies the input matrix in place.
     *
     * @param input The input matrix to be transformed.
     */
    void DiscreteCosineTransform::computeOpenMP(std::vector<std::vector<double>> &input){
        return dct::algorithms::compute_dct_2d_openMP(input);
    };
}