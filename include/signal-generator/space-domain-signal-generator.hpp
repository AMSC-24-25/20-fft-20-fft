#ifndef SPACE_DOMAIN_SIGNAL_GENERATOR_HPP
#define SPACE_DOMAIN_SIGNAL_GENERATOR_HPP
#include <complex>
#include <random>

#include "abstract-signal-generator.hpp"

class SpaceDomainSignalGenerator final : public AbstractSignalGenerator {
public:
    explicit SpaceDomainSignalGenerator(const std::optional<int> seed = std::nullopt) : AbstractSignalGenerator(seed) {}

    /**
     * Generate a signal of a given length in the space domain.
     * The values of the signal are complex numbers and are generated using a uniform distribution.
     *
     * Since the signal is in the space domain, which is a 2D domain,
     * the method performs a flattening operation so that the signal is returned as a 1D signal.
     *
     * The operation is performed using row-major order.
     * @param length Length of the signal.
     * @param lower_bound The lower bound for the uniform distribution.
     * @param upper_bound The upper bound for the uniform distribution.
     * @return The generated signal.
     */
    std::vector<std::complex<double>> generateSignal(int length, int lower_bound, int upper_bound) override;
};

#endif //SPACE_DOMAIN_SIGNAL_GENERATOR_HPP
