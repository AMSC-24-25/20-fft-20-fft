#ifndef SEQUENTIAL_1D_INVERSE_FAST_FT_HPP
#define SEQUENTIAL_1D_INVERSE_FAST_FT_HPP

#include "abstract-fourier-transform-solver.hpp"

class Sequential1DInverseFastFT final : public AbstractFourierTransformSolver {
public:
    /**
     * Initialize the solver with the complex input vector.
     * @param inputVector The input vector to be transformed.
     */
    explicit Sequential1DInverseFastFT (
        const std::vector<std::complex<double>> &inputVector
    ) : AbstractFourierTransformSolver(inputVector) {}

    /**
     * Run the sequential 1D Inverse Fast Fourier Transform solver.
     * It implements the Cooley-Tukey algorithm in an iterative way.
     */
    void compute() override;
};


#endif //SEQUENTIAL_1D_INVERSE_FAST_FT_HPP
