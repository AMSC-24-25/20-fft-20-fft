#include "inverse_discrete_cosine_transform.hpp"
#include "transforms/discrete_cosine_transform/algorithms/idct.hpp"
#include "transforms/discrete_cosine_transform/algorithms/idct_openmp.hpp"

namespace sp::dct::solver{
    /**
     * Internal method to compute the Inverse Discrete Cosine Transform in sequential mode.
     *
     * The input vector is transformed in place.
     *
     * @param input The input vector to be transformed.
     */
    void InverseDiscreteCosineTransform::computeSequential(std::vector<double> &input){
        return algo::computeIDCT1d(input);
    };

    /**
     * Internal method to compute the Inverse Discrete Cosine Transform in parallel mode (OpenMP).
     *
     * The input vector is transformed in place.
     *
     * @param input The input vector to be transformed.
     */
    void InverseDiscreteCosineTransform::computeOpenMP(std::vector<double> &input){
        return algo::computeIDCT1dOpenMP(input);
    };

    /**
     * Internal method to compute the Inverse Discrete Cosine Transform in sequential mode.
     *
     * The input matrix is transformed in place.
     *
     * @param input The input matrix to be transformed.
     */
    void InverseDiscreteCosineTransform::computeSequential(std::vector<std::vector<double>> &input){
        return algo::computeIDCT2d(input);
    };

    /**
     * Internal method to compute the Inverse Discrete Cosine Transform in parallel mode (OpenMP).
     *
     * The input matrix is transformed in place.
     *
     * @param input The input matrix to be transformed.
     */
    void InverseDiscreteCosineTransform::computeOpenMP(std::vector<std::vector<double>> &input){
        return algo::computeIDCT2dOpenMP(input);
    };
}
