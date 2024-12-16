#ifndef TIME_DOMAIN_SIGNAL_GENERATOR_HPP
#define TIME_DOMAIN_SIGNAL_GENERATOR_HPP
#include <complex>
#include <random>

#include "abstract-signal-generator.hpp"

class TimeDomainSignalGenerator final : public AbstractSignalGenerator {
public:
    explicit TimeDomainSignalGenerator(const std::optional<int> seed = std::nullopt) : AbstractSignalGenerator(seed) {}

    /**
     * Generate a signal of a given length in the time domain.
     * The values of the signal are generated using a uniform distribution and are complex numbers.
     * @param length Length of the signal.
     * @param lower_bound The lower bound for the uniform distribution.
     * @param upper_bound The upper bound for the uniform distribution.
     * @return The generated signal.
     */
    std::vector<std::complex<double>> generateSignal(int length, int lower_bound, int upper_bound) override;
};

#endif //TIME_DOMAIN_SIGNAL_GENERATOR_HPP
