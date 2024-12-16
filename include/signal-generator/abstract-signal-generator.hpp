#ifndef ABSTRACT_SIGNAL_GENERATOR_HPP
#define ABSTRACT_SIGNAL_GENERATOR_HPP
#include <optional>

/**
 * AbstractSignalGenerator is an abstract class that defines the interface for generating signals.
 */
class AbstractSignalGenerator {
protected:
    /**
     * Seed for the random generator.
     */
    std::optional<int> _seed;
    std::mt19937 _engine;

public:
    /**
     * The signal generator needs a _seed, which is used for the random generator and is constant.
     *
     * If the _seed is not provided, a random _seed is generated using std::random_device.
     * @param seed Seed for the random generator.
     */
    explicit AbstractSignalGenerator(const std::optional<int> seed = std::nullopt) : _seed(seed) {
        _engine.seed(_seed ? *_seed : std::random_device()());
    }

    virtual ~AbstractSignalGenerator() = default;

    /**
     * Generate a signal of a given length.
     * @param length The length of the signal.
     * @param lower_bound The lower bound for the uniform distribution.
     * @param upper_bound The upper bound for the uniform distribution.
     * @return The generated signal.
     */
    virtual std::vector<std::complex<double>> generateSignal(int length, int lower_bound, int upper_bound) = 0;

    /**
     * Get the _seed for the random generator.
     * @return The seed for the random generator.
     */
    [[nodiscard]] int getSeed() const {
        return _seed.value();
    }
};

#endif //ABSTRACT_SIGNAL_GENERATOR_HPP
