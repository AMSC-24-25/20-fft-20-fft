#ifndef INVERSE_FAST_FOURIER_TRANSFORM_HPP
#define INVERSE_FAST_FOURIER_TRANSFORM_HPP

#include "fourier_transform/base_fourier_transform.hpp"
#include "fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_inverse_fft_openmp.hpp"
#include "fourier_transform/algorithms/cooley_tukey/cooley_tukey_inverse_fft.hpp"
#ifdef HAS_CUDA
#include "fourier_transform/algorithms/cooley_tukey/cuda/cooley_tukey_fft_cuda.hpp"
#endif // HAS_CUDA

namespace fft::solver {
    /**
     * Inverse Fast Fourier Transform (IFFT) class.
     *
     * This class is a template for performing Inverse Fast Fourier Transforms (IFFT) in N dimensions.
     *
     * The solver can be used in two modes:
     *  - SEQUENTIAL: For sequential execution.
     *  - OPENMP: For parallel execution using OpenMP.
     *
     * @tparam N Dimensions of the IFFT (1D, 2D, etc.), obviously cannot be less than 1.
     */
    template <size_t N>
    class InverseFastFourierTransform final : public BaseFourierTransform<N> {
    public:
        /**
         * Create an Inverse Fast Fourier Transform solver.
         *
         * @param dimensions An array of dimensions for the IFFT.
         */
        explicit InverseFastFourierTransform(const std::array<size_t, N>& dimensions)
            : BaseFourierTransform<N>(dimensions) {}
    protected:
        /**
         * Get the sequential transform function.
         *
         * This method provides the specific implementation of the Inverse Fast Fourier Transform
         * using a sequential algorithm.
         */
        [[nodiscard]] typename InverseFastFourierTransform::transform_t getSequentialTransform() const override {
            return [](std::vector<std::complex<double>>& data) {
                algorithms::cooley_tukey::computeInverseFFT(data);
            };
        }

        /**
         * Get the OpenMP transform function.
         *
         * This method provides the specific implementation of the Inverse Fast Fourier Transform
         * using OpenMP for parallel execution.
         */
        [[nodiscard]] typename InverseFastFourierTransform::transform_t getOpenMPTransform() const override {
            return [](std::vector<std::complex<double>>& data) {
                algorithms::cooley_tukey::computeInverseFFTOpenMP(data);
            };
        }

        /**
         * Get the CUDA transform function.
         *
         * This method provides the specific implementation of the Inverse Fast Fourier Transform
         * using CUDA for parallel execution.
         *
         * @throws std::runtime_error if CUDA is not available on the machine.
         */
        [[nodiscard]] typename InverseFastFourierTransform::transform_t getCudaTransform() const override {
            return [](std::vector<std::complex<double>>& data) {
                #ifdef HAS_CUDA
                algorithms::cooley_tukey::computeInverseFFTCuda(data);
                #else // HAS_CUDA
                throw std::runtime_error("CUDA is not available in this machine, please use another mode.");
                #endif // HAS_CUDA
            };
        }
    };
}

#endif //INVERSE_FAST_FOURIER_TRANSFORM_HPP
