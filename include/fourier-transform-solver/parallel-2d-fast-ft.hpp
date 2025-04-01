#ifndef PARALLEL_2D_FAST_FOURIER_TRANSFORM_HPP
#define PARALLEL_2D_FAST_FOURIER_TRANSFORM_HPP

#include "abstract-fourier-transform-solver.hpp"

class Parallel2DFastFT final {
    bool isComputed = false;
protected:
    /**
     * Input vector to be transformed.
     */
    std::vector<std::vector<std::complex<double>>> input;
    /**
     * Solution vector after transformation.
     */
    std::optional<std::vector<std::vector<std::complex<double>>>> solution = std::nullopt;
public:
    virtual ~Parallel2DFastFT() = default;

    /**
     * Initialize the solver with the complex input vector.
     * @param inputVector The input vector to be transformed.
     */
    explicit Parallel2DFastFT (
        const std::vector<std::vector<std::complex<double>>> &inputVector
    ) {}

    /**
     * Run the Parallel 1D Fast Fourier Transform solver.
     * It implements the Cooley-Tukey algorithm in an iterative way (parallel).
     * The algorithm is parallelized using OpenMP.
     */
    void compute();

    const std::vector<std::vector<std::complex<double>>> &getSolution() const;
};

#endif //PARALLEL_2D_FAST_FOURIER_TRANSFORM_HPP
