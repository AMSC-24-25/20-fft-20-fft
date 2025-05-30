#ifndef BASE_SIGNAL_GENERATOR_HPP
#define BASE_SIGNAL_GENERATOR_HPP

#include <random>
#include <complex>

#include "utils/legacy_support.hpp"

/**
 * Signal Generator module.
 *
 * This module is responsible for generating signals in the signal processing domain.
 *
 * It is a utility module that provides a way to create synthetic signals for testing and analysis.
 */
namespace sp::signal_gen
{
    /**
     * BaseSignalGenerator is an abstract class that defines the interface for generating signals.
     */
    class BaseSignalGenerator {
    protected:
        /**
         * Seed for the random generator.
         */
        utils::legacy::Optional<int> _seed;
        /**
         * Random engine for the random generator.
         * The Mersenne Twister is a general-purpose pseudorandom number generator (PRNG).
         *
         * It produces 32-bit pseudo-random numbers using the well-known and
         * popular algorithm named Mersenne Twister algorithm.
         *
         * The word mt19937 stands for Mersenne Twister with a long period of 2^{19937} – 1 which means
         * mt19937 produces a sequence of 32-bit integers that only repeats itself
         * after 2^{19937} – 1 number have been generated.
         */
        std::mt19937 _engine;

    public:
        /**
         * The signal generator needs a _seed, which is used for the random generator and is constant.
         *
         * @param seed Seed for the random generator.
         */
        explicit BaseSignalGenerator(const int seed) : _seed(seed) {
            _engine.seed(_seed.value());
        }

        /**
         * A random seed is generated using the std::random_device.
         */
        explicit BaseSignalGenerator() {
            _engine.seed(std::random_device()());
        }

        virtual ~BaseSignalGenerator() = default;

        /**
         * Generate a random (thanks to noise) one-dimensional complex signal.
         *
         * The generation is based on a Gaussian distribution with a mean of 0 and a standard deviation equal to the noise.
         *  - The mean (mu) is 0, so the noise is centered around zero and does not distort the signal.
         *  - The standard deviation (sigma) is equal to the noise because it determines
         *    how the noise values are spread around the mean.
         *    Approximately 68% of the noise values will fall within +-sigma of the mean, 95% will fall within +-2*sigma,
         *    and 99.7% will fall within +-3*sigma.
         *
         * @param length The length of the signal. Specifies the number of samples or data points in the signal.
         * @param frequency Represents the frequency of the signal's oscillations. It depends on the signal domain.
         *                  For example, it refers to the number of cycles per second (hertz) in the time domain,
         *                  and it indicates spatial frequency (or the number of cycles per unit distance)
         *                  in the spatial domain.
         * @param phase The phase (radians) of the signal.
         *              It allows you to move the signal along the time axis (i.e. to the left or right).
         *              It represents the initial angle of the sine or cosine function at time t = 0,
         *              or the shift of the signal waveform.
         * @param noise Thanks to the noise parameter, the signal can be randomly distorted.
         *              In addition, the noise guarantees a more realistic signal generation.
         * @return The generated signal (complex signal).
         */
        virtual std::vector<std::complex<double>> generate1DSignal(
            int length, double frequency, double phase, double noise
        ) = 0;

        /**
         * Generate a random (thanks to noise) one-dimensional real signal.
         *
         * The generation is based on a Gaussian distribution with a mean of 0 and a standard deviation equal to the noise.
         *  - The mean (mu) is 0, so the noise is centered around zero and does not distort the signal.
         *  - The standard deviation (sigma) is equal to the noise because it determines
         *    how the noise values are spread around the mean.
         *    Approximately 68% of the noise values will fall within +-sigma of the mean, 95% will fall within +-2*sigma,
         *    and 99.7% will fall within +-3*sigma.
         *
         * @param length The length of the signal. Specifies the number of samples or data points in the signal.
         * @param frequency Represents the frequency of the signal's oscillations. It depends on the signal domain.
         *                  For example, it refers to the number of cycles per second (hertz) in the time domain,
         *                  and it indicates spatial frequency (or the number of cycles per unit distance)
         *                  in the spatial domain.
         * @param phase The phase (radians) of the signal.
         *              It allows you to move the signal along the time axis (i.e. to the left or right).
         *              It represents the initial angle of the sine or cosine function at time t = 0,
         *              or the shift of the signal waveform.
         * @param noise Thanks to the noise parameter, the signal can be randomly distorted.
         *              In addition, the noise guarantees a more realistic signal generation.
         * @return The generated signal (real signal).
         */
        virtual std::vector<double> generateReal1DSignal(
            int length, double frequency, double phase, double noise
        ) = 0;

        /**
         * Get the _seed for the random generator.
         * @return The seed for the random generator.
         */
        [[nodiscard]] int getSeed() const {
            return _seed.value();
        }
    };
}

#endif //BASE_SIGNAL_GENERATOR_HPP
