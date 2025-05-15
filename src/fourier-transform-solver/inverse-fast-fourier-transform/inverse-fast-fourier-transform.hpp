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
    public:
        /**
         * Create an Inverse Fast Fourier Transform solver.
         *
         * @param dimensions An array of dimensions for the IFFT.
         */
        explicit InverseFastFourierTransform(const std::array<size_t, N>& dimensions)
            : FourierTransformSolver<N>(dimensions) {}
    protected:
        /**
         * Get the sequential transform function.
         *
         * This method provides the specific implementation of the Inverse Fast Fourier Transform
         * using a sequential algorithm.
         */
        [[nodiscard]] typename InverseFastFourierTransform::transform_t getSequentialTransform() const override {
            return [](std::vector<std::complex<double>>& data) {
                algorithms::cooley_tukey::computeCooleyTurkeyInverseFFT(data);
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
                algorithms::cooley_tukey::computeCooleyTurkeyInverseFFTOpenMP(data);
            };
        }
    };
}

#endif //INVERSE_FAST_FOURIER_TRANSFORM_HPP
