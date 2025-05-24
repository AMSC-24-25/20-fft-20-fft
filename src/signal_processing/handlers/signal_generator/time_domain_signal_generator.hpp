#ifndef TIME_DOMAIN_SIGNAL_GENERATOR_HPP
#define TIME_DOMAIN_SIGNAL_GENERATOR_HPP
#include <complex>
#include <random>

#include "base_signal_generator.hpp"

namespace sp::signal_gen
{
    class TimeDomainSignalGenerator final : public BaseSignalGenerator {
    public:
        explicit TimeDomainSignalGenerator(const std::optional<int> seed = std::nullopt) : BaseSignalGenerator(seed) {}

        /**
         * Generate a random (thanks to noise) one-dimensional signal in the time domain.
         *
         * Let the angular frequency be:
         * @code
         *      (omega) w = 2 * M_PI * frequency
         * @endcode
         * Where:
         *  - @code M_PI@endcode is the mathematical constant pi.
         *  - @code frequency@endcode is the frequency of the signal in seconds.
         *    It is the number of cycles that occur in one second (measured in Hertz, Hz).
         *  - @code 2 * M_PI@endcode is one complete cycle of a sine wave (in our case), which is 2*pi radians
         *    (note: a circle representing one complete oscillation has 2*pi radians).
         *  - @code 2 * M_PI@endcode is multiplied by @code frequency@endcode to get the angular frequency.
         *    So to find out how many radians are covered per second, we use this multiplication.
         *    In other words, it converts the frequency from cycles per second (Hz)
         *    to angular frequency in radians per second.
         *
         * The real part of the signal is generated using the cosine function:
         * @code
         *      cos(omega * time + phase) + noise
         * @endcode
         * Where time is the discrete time step at iteration i.
         *
         * Similarly, the imaginary part is generated using the sine function:
         * @code
         *      sin(omega * time + phase) + noise
         * @endcode
         *
         * A random noise is added to each formula to distort the signal.
         *
         * @param length The length of the signal. It represents the duration of the signal in discrete time steps.
         * @param frequency The frequency (Hz) of the signal.
         *                  In other words, it determines how many cycles (oscillations) the signal makes per second.
         * @param phase The phase (radians) of the signal.
         *              It represents the initial angle of the sine or cosine function at time t = 0.
         * @param noise The range of the random noise added to each sample of the signal.
         * @return The generated signal.
         */
        std::vector<std::complex<double>> generate1DSignal(
            int length, double frequency, double phase, double noise
        ) override;
    };
}

#endif //TIME_DOMAIN_SIGNAL_GENERATOR_HPP
