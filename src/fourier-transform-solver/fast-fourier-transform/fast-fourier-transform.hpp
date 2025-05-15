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
    public:
        /**
         * Create a Fast Fourier Transform solver.
         *
         * @param dimensions An array of dimensions for the FFT.
         */
        explicit FastFourierTransform(const std::array<size_t, N>& dimensions)
            : FourierTransformSolver<N>(dimensions) {}
    protected:
        /**
         * Get the sequential transform function.
         *
         * This method provides the specific implementation of the Fast Fourier Transform
         * using a sequential algorithm.
         */
        [[nodiscard]] typename FastFourierTransform::transform_t getSequentialTransform() const override {
            return [](std::vector<std::complex<double>>& data) {
                algorithms::cooley_tukey::computeCooleyTurkeyFFT(data);
            };
        }

        /**
         * Get the OpenMP transform function.
         *
         * This method provides the specific implementation of the Fast Fourier Transform
         * using OpenMP for parallel execution.
         */
        [[nodiscard]] typename FastFourierTransform::transform_t getOpenMPTransform() const override {
            return [](std::vector<std::complex<double>>& data) {
                algorithms::cooley_tukey::computeCooleyTurkeyFFTOpenMP(data);
            };
        }
    };
}

#endif //FAST_FOURIER_TRANSFORM_HPP
