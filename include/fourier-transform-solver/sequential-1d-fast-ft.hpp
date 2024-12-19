#ifndef SEQUENTIAL_1D_FAST_FOURIER_TRANSFORM_HPP
#define SEQUENTIAL_1D_FAST_FOURIER_TRANSFORM_HPP

#include "abstract-fourier-transform-solver.hpp"


class Sequential1DFastFT final : public AbstractFourierTransformSolver {
public:
    /**
     * Initialize the solver with the complex input vector.
     * @param inputVector The input vector to be transformed.
     */
    explicit Sequential1DFastFT (
        const std::vector<std::complex<double>> &inputVector
    ) : AbstractFourierTransformSolver(inputVector) {}

    /**
     * Run the sequential 1D Fast Fourier Transform solver.
     */
    void compute() override;
};

#endif //SEQUENTIAL_1D_FAST_FOURIER_TRANSFORM_HPP
