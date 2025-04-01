#ifndef ABSTRACT_FOURIER_TRANSFORM_SOLVER_HPP
#define ABSTRACT_FOURIER_TRANSFORM_SOLVER_HPP
#include <complex>
#include <optional>
#include <vector>

/**
 * Abstract class for Fourier Transform solvers.
 */
class AbstractFourierTransformSolver {
    bool isComputed = false;
protected:
    /**
     * Input vector to be transformed.
     */
    std::vector<std::complex<double>> input;
    /**
     * Solution vector after transformation.
     */
    std::optional<std::vector<std::complex<double>>> solution = std::nullopt;
public:
    // AbstractFourierTransformSolver() = default;
    virtual ~AbstractFourierTransformSolver() = default;

    /**
     * Initialize the solver with the complex input vector.
     * @param inputVector The input vector to be transformed.
     */
    explicit AbstractFourierTransformSolver(
        const std::vector<std::complex<double>> &inputVector
    ) : input(inputVector) {}

    /**
     * Get the output vector after transformation if it exists.
     * @return The output vector after transformation.
     * @throw std::runtime_error If the solution does not exist.
     */
    [[nodiscard]] const std::vector<std::complex<double>> &getSolution() const {
        if (!solution.has_value()) {
            throw std::runtime_error("Solution does not exist. Run the solver first.");
        }
        return solution.value();
    }

    /**
     * Run the solver to compute the Fourier Transform.
     */
    virtual void compute() = 0;
};

#endif //ABSTRACT_FOURIER_TRANSFORM_SOLVER_HPP
