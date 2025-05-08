#ifndef INVERSE_DISCRETE_COSINE_TRANSFORM_HPP
#define INVERSE_DISCRETE_COSINE_TRANSFORM_HPP

#include "discrete-cosine-transform-solver/discrete-cosine-transform-solver.hpp"
#include "discrete-cosine-transform-solver/algorithms/idct.hpp"
#include "discrete-cosine-transform-solver/algorithms/idct_openMP.hpp"
#include <vector>

namespace dct::solver {
    /**
     * Inverse Discrete Cosine Transform (IDCT) class.
     *
     * This class performs Inverse Discrete Cosine Transform (IDCT).
     *
     * The solver can be used in two modes:
     *  - SEQUENTIAL: For sequential execution.
     *  - OPENMP: For parallel execution using OpenMP.
     */
    class InverseDiscreteCosineTransform final : public DiscreteCosineTransformSolver {
    protected:
        /**
         * Internal method to compute the Inverse Discrete Cosine Transform in sequential mode.
         *
         * The input vector is transformed in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeSequential(std::vector<double> &input) override;

        /**
         * Internal method to compute the Inverse Discrete Cosine Transform in parallel mode (OpenMP).
         *
         * The input vector is transformed in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeOpenMP(std::vector<double> &input) override;

        /**
         * Internal method to compute the Inverse Discrete Cosine Transform in sequential mode.
         *
         * The input matrix is transformed in place.
         *
         * @param input The input matrix to be transformed.
         */
        void computeSequential(std::vector<std::vector<double>> &input);

        /**
         * Internal method to compute the Inverse Discrete Cosine Transform in parallel mode (OpenMP).
         *
         * The input matrix is transformed in place.
         *
         * @param input The input matrix to be transformed.
         */
        void computeOpenMP(std::vector<std::vector<double>> &input) override;
    };
}
#endif //INVERSE_DISCRETE_COSINE_TRANSFORM_HPP