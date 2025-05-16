#include <stdexcept>

#include "base_discrete_cosine_transform.hpp"

namespace dct::solver {
    /**
     * Compute the Discrete Cosine Transform of the input vector (in-place).
     *
     * The input vector will be modified to store the result of the transformation.
     *
     * @param input The input vector to be transformed.
     * @param mode The mode of computation.
     * @param mode The mode of computation.
     */
    void DiscreteCosineTransformSolver::compute(std::vector<double> &input, const ComputationMode mode) {
        if (input.empty()) {
            throw std::invalid_argument("Input vector is empty.");
        }
        if (mode == ComputationMode::SEQUENTIAL) {
            this->computeSequential(input);
        } else if (mode == ComputationMode::OPENMP) {
            this->computeOpenMP(input);
        } else {
            throw std::invalid_argument("Invalid mode specified.");
        }
    };

    /**
     * Compute the Discrete Cosine Transform of the input vector and store the result in the output vector.
     *
     * The input vector will not be modified.
     *
     * @param input The input vector to be transformed.
     * @param output The output vector after transformation.
     */
    void DiscreteCosineTransformSolver::compute(const std::vector<double> &input, std::vector<double> &output, const ComputationMode mode) {
        output = input;
        this->compute(output, mode);
    };

    /**
     * Compute the Discrete Cosine Transform of the input vector (in-place).
     *
     * The input vector will be modified to store the result of the transformation.
     *
     * @param input The input matrix to be transformed.
     * @param mode The mode of computation.
     * @param mode The mode of computation.
     */
    void DiscreteCosineTransformSolver::compute(std::vector<std::vector<double>>&input, const ComputationMode mode) {
        if (input.empty()) {
            throw std::invalid_argument("Input vector is empty.");
        }
        if (mode == ComputationMode::SEQUENTIAL) {
            this->computeSequential(input);
        } else if (mode == ComputationMode::OPENMP) {
            this->computeOpenMP(input);
        } else {
            throw std::invalid_argument("Invalid mode specified.");
        }
    };

    /**
     * Compute the Discrete Cosine Transform of the input vector and store the result in the output vector.
     *
     * The input vector will not be modified.
     *
     * @param input The input matrix to be transformed.
     * @param output The output vector after transformation.
     */
    void DiscreteCosineTransformSolver::compute(std::vector<std::vector<double>> &input, std::vector<std::vector<double>> &output, const ComputationMode mode) {
        output = input;
        this->compute(output, mode);
    };

}
