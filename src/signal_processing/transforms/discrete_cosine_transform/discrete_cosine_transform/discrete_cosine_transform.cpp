#include "discrete_cosine_transform.hpp"
#include "transforms/discrete_cosine_transform/algorithms/dct.hpp"
#include "transforms/discrete_cosine_transform/algorithms/dct_openmp.hpp"

namespace sp::dct::solver {
    /**
     * Internal method to compute the Discrete Cosine Transform in sequential mode.
     *
     * This method modifies the input vector in place.
     *
     * @param input The input vector to be transformed.
     */
    void DiscreteCosineTransform::computeSequential(std::vector<double> &input){
        return algo::computeDCT1d(input);
    };

    /**
     * Internal method to compute the Discrete Cosine Transform in parallel mode using OpenMP.
     *
     * This method modifies the input vector in place.
     *
     * @param input The input vector to be transformed.
     */
    void DiscreteCosineTransform::computeOpenMP(std::vector<double> &input){
        return algo::computeDCT1dOpenMP(input);
    };

    /**
     * Internal method to compute the Discrete Cosine Transform in sequential mode.
     *
     * This method modifies the input matrix in place.
     *
     * @param input The input matrix to be transformed.
     */
    void DiscreteCosineTransform::computeSequential(std::vector<std::vector<double>> &input){
        return algo::computeDCT2d(input);
    };

    /**
     * Internal method to compute the Discrete Cosine Transform in parallel mode using OpenMP.
     *
     * This method modifies the input matrix in place.
     *
     * @param input The input matrix to be transformed.
     */
    void DiscreteCosineTransform::computeOpenMP(std::vector<std::vector<double>> &input){
        return algo::computeDCT2dOpenMP(input);
    };
}
