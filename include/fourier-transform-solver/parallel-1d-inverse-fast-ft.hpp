#ifndef PARALLEL_1D_INVERSE_FFT_HPP
#define PARALLEL_1D_INVERSE_FFT_HPP

#include "abstract-fourier-transform-solver.hpp"

class Parallel1DInverseFastFT final : public AbstractFourierTransformSolver {
public:
    /**
     * Initialize the solver with the complex input vector.
     * @param inputVector The input vector to be transformed.
     */
    explicit Parallel1DInverseFastFT (
        const std::vector<std::complex<double>> &inputVector
    ) : AbstractFourierTransformSolver(inputVector) {}

    /**
     * Run the parallel 1D Inverse Fast Fourier Transform solver.
     * It implements the Cooley-Tukey algorithm in an iterative way (parallel).
     * The algorithm is parallelized using OpenMP.
     */
    void compute() override;
};

#endif //PARALLEL_1D_INVERSE_FFT_HPP
