#ifndef BASE_DISCRETE_COSINE_TRANSFORM_HPP
#define BASE_DISCRETE_COSINE_TRANSFORM_HPP
#include <vector>

namespace sp::dct::solver {
    /**
     * Enumeration for the modes of computation.
     *
     * The modes are:
     *  - <code>SEQUENTIAL</code>: Sequential computation.
     *  - <code>OPENMP</code>: Parallel computation using OpenMP.
     */
    enum class ComputationMode {
        SEQUENTIAL,
        OPENMP
    };

    /**
     * Abstract class for Discrete Cosine Transform solvers.
     *
     * This class provides an interface for computing the Discrete Cosine
     * Transform of a given input vector.It supports different modes of computation,
     * including sequential and parallel (OpenMP) modes.
     */
    class DiscreteCosineTransformSolver {
    public:
        virtual ~DiscreteCosineTransformSolver() = default;

        /**
         * Compute the Discrete Cosine Transform of the input vector (in-place).
         *
         * The input vector will be modified to store the result of the transformation.
         *
         * @param input The input vector to be transformed.
         * @param mode The mode of computation.
         * @param mode The mode of computation.
         */
        void compute(std::vector<double> &input, const ComputationMode mode);

        /**
         * Compute the Discrete Cosine Transform of the input vector and store the result in the output vector.
         *
         * The input vector will not be modified.
         *
         * @param input The input vector to be transformed.
         * @param output The output vector after transformation.
         */
        void compute(const std::vector<double> &input, std::vector<double> &output, const ComputationMode mode);

        /**
         * Compute the Discrete Cosine Transform of the input vector (in-place).
         *
         * The input vector will be modified to store the result of the transformation.
         *
         * @param input The input matrix to be transformed.
         * @param mode The mode of computation.
         * @param mode The mode of computation.
         */
        void compute(std::vector<std::vector<double>>&input, const ComputationMode mode);

        /**
         * Compute the Discrete Cosine Transform of the input vector and store the result in the output vector.
         *
         * The input vector will not be modified.
         *
         * @param input The input matrix to be transformed.
         * @param output The output vector after transformation.
         */
        void compute(
            const std::vector<std::vector<double>> &input,
            std::vector<std::vector<double>> &output,
            const ComputationMode mode
        );

    protected:
        /**
         * Internal method to compute the Discrete Cosine Transform in sequential mode.
         *
         * It modifies the input vector in-place.
         *
         * @param input The input vector to be transformed.
         */
        virtual void computeSequential(std::vector<double>& input) = 0;

        /**
         * Internal method to compute the Discrete Cosine Trasform in parallel mode using OpenMP.
         *
         * It modifies the input vector in-place.
         *
         * @param input The input vector to be transformed.
         */
        virtual void computeOpenMP(std::vector<double>& input) = 0;

        /**
         * Internal method to compute the Discrete Cosine Transform in sequential mode.
         *
         * It modifies the input matrix in-place.
         *
         * @param input The input matrix to be transformed.
         */
        virtual void computeSequential(std::vector<std::vector<double>>& input) = 0;

        /**
         * Internal method to compute the Discrete Cosine Trasform in parallel mode using OpenMP.
         *
         * It modifies the input matrix in-place.
         *
         * @param input The input matrix to be transformed.
         */
        virtual void computeOpenMP(std::vector<std::vector<double>>& input) = 0;
    };
}
#endif //BASE_DISCRETE_COSINE_TRANSFORM_HPP
