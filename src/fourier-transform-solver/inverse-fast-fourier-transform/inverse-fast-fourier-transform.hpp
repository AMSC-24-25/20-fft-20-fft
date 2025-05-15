#ifndef INVERSE_FAST_FOURIER_TRANSFORM_HPP
#define INVERSE_FAST_FOURIER_TRANSFORM_HPP

#include "fourier-transform-solver/fourier-transform-solver.hpp"
#include "fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft-openmp.hpp"
#include "fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft.hpp"

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
    class InverseFastFourierTransform final : public FourierTransformSolver<N> {
    protected:
        /**
         * Internal method to compute the Inverse Fast Fourier Transform in sequential mode.
         *
         * The input vector is transformed in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeSequential(
            std::vector<std::complex<double>>& input
        ) override {
            if (N == 1) {
                return algorithms::cooley_tukey::computeCooleyTurkeyInverseFFT(input);
            }
            throw std::runtime_error("Inverse FFT not implemented for N > 1");
        }

        /**
         * Internal method to compute the Inverse Fast Fourier Transform in parallel mode (OpenMP).
         *
         * The input vector is transformed in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeOpenMP(
            std::vector<std::complex<double>>& input
        ) override {
            if (N == 1) {
                return algorithms::cooley_tukey::computeCooleyTurkeyInverseFFTOpenMP(input);
            }
            throw std::runtime_error("Inverse FFT not implemented for N > 1");
        }
    };
}

#endif //INVERSE_FAST_FOURIER_TRANSFORM_HPP
