#ifndef FAST_FOURIER_TRANSFORM_HPP
#define FAST_FOURIER_TRANSFORM_HPP

#include "fourier-transform-solver/fourier-transform-solver.hpp"
#include "fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft-openmp.hpp"
#include "fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft.hpp"

namespace fft::solver {
    /**
    * Fast Fourier Transform (FFT) class.
    *
    * This class is a template for performing Fast Fourier Transforms (FFT) in N dimensions.
    *
    * The solver can be used in two modes:
    *  - SEQUENTIAL: For sequential execution.
    *  - OPENMP: For parallel execution using OpenMP.
    *
    * @tparam N Dimensions of the FFT (1D, 2D, etc.), obviously cannot be less than 1.
    */
    template <size_t N>
    class FastFourierTransform final : public FourierTransformSolver<N> {
    protected:
        /**
         * Internal method to compute the Fast Fourier Transform in sequential mode.
         *
         * This method modifies the input vector in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeSequential(
            std::vector<std::complex<double>>& input
        ) override {
            if (N == 1) {
                return algorithms::cooley_tukey::computeCooleyTurkeyFFT(input);
            }
            throw std::runtime_error("FFT not implemented for N > 1");
        }

        /**
         * Internal method to compute the Fast Fourier Transform in parallel mode using OpenMP.
         *
         * This method modifies the input vector in place.
         *
         * @param input The input vector to be transformed.
         */
        void computeOpenMP(
            std::vector<std::complex<double>>& input
        ) override {
            if (N == 1) {
                return algorithms::cooley_tukey::computeCooleyTurkeyFFTOpenMP(input);
            }
            throw std::runtime_error("FFT not implemented for N > 1");
        }
    };
}

#endif //FAST_FOURIER_TRANSFORM_HPP
