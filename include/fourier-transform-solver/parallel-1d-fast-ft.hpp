#ifndef PARALLEL_1D_FAST_FOURIER_TRANSFORM_HPP
#define PARALLEL_1D_FAST_FOURIER_TRANSFORM_HPP

#include "abstract-fourier-transform-solver.hpp"

class Parallel1DFastFT final : public AbstractFourierTransformSolver {
public:
    /**
     * Initialize the solver with the complex input vector.
     * @param inputVector The input vector to be transformed.
     */
    explicit Parallel1DFastFT (
        const std::vector<std::complex<double>> &inputVector
    ) : AbstractFourierTransformSolver(inputVector) {}

    /**
     * Run the Parallel 1D Fast Fourier Transform solver.
     * It implements the Cooley-Tukey algorithm in an iterative way (parallel).
     * The algorithm is parallelized using OpenMP.
     */
    void compute() override;
};

#endif //PARALLEL_1D_FAST_FOURIER_TRANSFORM_HPP
