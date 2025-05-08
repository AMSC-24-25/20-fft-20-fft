#ifndef FOURIER_TRANSFORM_SOLVER_HPP
#define FOURIER_TRANSFORM_SOLVER_HPP
#include <complex>
#include <vector>

namespace fft::solver
{
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
     * Abstract template class for Fourier Transform solvers.
     *
     * This class provides an interface for computing the Fourier Transform
     * of a given input vector. It supports different modes of computation,
     * including sequential and parallel (OpenMP) modes.
     */
    template <size_t N>
    class FourierTransformSolver {
    public:
        static_assert(
            N >= 1,
            "N must be at least 1, it represents the number of dimensions for the FFT (1D, 2D, etc.)"
        );

        virtual ~FourierTransformSolver() = default;

        /**
         * Compute the Fourier Transform of the input vector (in-place).
         *
         * The input vector will be modified to store the result of the transformation.
         *
         * @param input The input vector to be transformed.
         * @param mode The mode of computation.
         */
        void compute(
            std::vector<std::complex<double>> &input,
            const ComputationMode mode
        ) {
            if (input.empty()) {
                throw std::invalid_argument("Input vector is empty.");
            }
            if ((input.size() & input.size() - 1) != 0) {
                throw std::invalid_argument(
                    "Input vector size is not a power of 2. This constraint is required for Cooley-Tukey FFT algorithm."
                );
            }
            if (mode == ComputationMode::SEQUENTIAL) {
                this->computeSequential(input);
            } else if (mode == ComputationMode::OPENMP) {
                this->computeOpenMP(input);
            } else {
                throw std::invalid_argument("Invalid mode specified.");
            }
        }

        /**
         * Compute the Fourier Transform of the input vector and store the result in the output vector.
         *
         * The input vector will not be modified.
         *
         * @param input The input vector to be transformed.
         * @param output The output vector after transformation.
         * @param mode The mode of computation.
         */
        void compute(
            const std::vector<std::complex<double>> &input,
            std::vector<std::complex<double>> &output,
            const ComputationMode mode
        ) {
            output = input;
            this->compute(output, mode);
        }

    protected:
        /**
         * Internal method to compute the Fourier Transform in sequential mode.
         *
         * It modifies the input vector in-place.
         *
         * @param input The input vector to be transformed.
         */
        virtual void computeSequential(
            std::vector<std::complex<double>>& input
        ) = 0;

        /**
         * Internal method to compute the Fourier Transform in parallel mode using OpenMP.
         *
         * It modifies the input vector in-place.
         *
         * @param input The input vector to be transformed.
         */
        virtual void computeOpenMP(
            std::vector<std::complex<double>>& input
        ) = 0;
    };
}


#endif //FOURIER_TRANSFORM_SOLVER_HPP
