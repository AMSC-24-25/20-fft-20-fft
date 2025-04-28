#ifndef FOURIER_TRANSFORM_SOLVER_HPP
#define FOURIER_TRANSFORM_SOLVER_HPP
#include <complex>
#include <vector>
#include <numeric>

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
            N > 0,
            "The number of dimensions N must be greater than 0."
        );

        /**
         * The dimensions of the input vector.
         * This is a fixed-size array of size N,
         * where each element represents the size of the corresponding dimension.
         *
         * For example, for a 2D FFT,
         * dimensions[0] would be the number of rows and dimensions[1] would be the number of columns.
         *
         * @note The number of dimensions is equal to the template parameter N.
         * @warning The dimensions must be powers of 2 for the Cooley-Tukey FFT algorithm to work correctly.
         */
        const std::array<size_t, N> dims;

        /**
         * Create a Fourier Transform solver.
         *
         * @param dimensions An array of dimensions for the Fourier Transform.
         * @throws std::invalid_argument if the number of dimensions is not equal to N.
         * @throws std::invalid_argument if the dimensions are not powers of 2.
         */
        explicit FourierTransformSolver(const std::array<size_t, N>& dimensions): dims(dimensions) {
            if (dimensions.size() != N) {
                throw std::invalid_argument(
                    "The number of dimensions must match the template parameter N."
                );
            }
            // check if all dimensions are positive powers of 2
            // (i.e., greater than 0 and a power of 2)
            // static_assert is not used here because it would require the dimensions to be known at compile time
            // and the dimensions are passed at runtime
            for (const size_t &dim : dimensions) {
                if (dim <= 0 || (dim & dim - 1) != 0) {
                    throw std::invalid_argument(
                        "All dimensions must be positive powers of 2."
                    );
                }
            }
        }

        virtual ~FourierTransformSolver() = default;

        /**
         * Compute the Fourier Transform of the input vector (in-place).
         *
         * The input vector will be modified to store the result of the transformation.
         *
         * @param input The input vector to be transformed.
         * @param mode The mode of computation.
         * @throws std::invalid_argument if the input vector size does not match the expected size (based on dimensions).
         */
        void compute(
            std::vector<std::complex<double>> &input,
            const ComputationMode mode
        ) {
            // check if the size of the input vector corresponds to the multiplication of each element of dims
            const size_t expected_size = std::accumulate(
                dims.begin(), dims.end(), 1, std::multiplies()
            );
            if (input.size() != expected_size) {
                throw std::invalid_argument(
                    "Input vector size does not match the expected size based on dimensions. Given: " +
                    std::to_string(input.size()) + ", Expected: " +
                    std::to_string(expected_size)
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
