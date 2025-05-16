#ifndef DISCRETE_COSINE_TRANSFORM_HPP
#define DISCRETE_COSINE_TRANSFORM_HPP

#include "discrete_cosine_transform/base_discrete_cosine_transform.hpp"
#include "discrete_cosine_transform/algorithms/dct.hpp"
#include "discrete_cosine_transform/algorithms/dct_openmp.hpp"
#include <vector>

namespace dct::solver {
    /**
     * Discrete Cosine Transform (DCT) class.
     *
     * This class performs Discrete Cosine Transform (DCT) in one or two dimensions.
     *
    */
    class DiscreteCosineTransform final : public DiscreteCosineTransformSolver {
    protected:
        /**
         * Internal method to compute the Discrete Cosine Transform in sequential mode.
         *
         * This method modifies the input vector in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeSequential(std::vector<double> &input) override;

        /**
         * Internal method to compute the Discrete Cosine Transform in parallel mode using OpenMP.
         *
         * This method modifies the input vector in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeOpenMP(std::vector<double> &input) override;

        /**
         * Internal method to compute the Discrete Cosine Transform in sequential mode.
         *
         * This method modifies the input matrix in place.
         *
         * @param input The input matrix to be transformed.
         */
        void computeSequential(std::vector<std::vector<double>> &input) override;

        /**
         * Internal method to compute the Discrete Cosine Transform in parallel mode using OpenMP.
         *
         * This method modifies the input matrix in place.
         *
         * @param input The input matrix to be transformed.
         */
        void computeOpenMP(std::vector<std::vector<double>> &input) override;
    };
}
#endif //DISCRETE_COSINE_TRANSFORM_HPP