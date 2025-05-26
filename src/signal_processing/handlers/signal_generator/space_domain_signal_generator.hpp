#ifndef SPACE_DOMAIN_SIGNAL_GENERATOR_HPP
#define SPACE_DOMAIN_SIGNAL_GENERATOR_HPP
#include <complex>
#include <random>

#include "base_signal_generator.hpp"

namespace sp::signal_gen
{
    class SpaceDomainSignalGenerator final : public BaseSignalGenerator {
    public:
        explicit SpaceDomainSignalGenerator(const std::optional<int> seed = std::nullopt) : BaseSignalGenerator(seed) {}

        /**
         * Generate a random (thanks to noise) one-dimensional signal in the space domain.
         *
         * Let the spatial frequency be k = frequency.
         *
         * Let the unique spatial coordinate be x (since it's 1D).
         *
         * Let the phase shift be phi.
         *
         * The signal is generated using the formula:
         * @code
         *      signal(x) = (cos(2 * M_PI * k * x + phi) + noise) + j * (sin(2 * M_PI * k * x + phi) + noise)
         * @endcode
         * Where j is the imaginary unit.
         *
         * Using the parameters name of the function, the formula becomes:
         * @code
         *      signal(x) = (cos(2 * M_PI * frequency * x + phase) + noise) + j * (sin(2 * M_PI * frequency * x + phase) + noise)
         * @endcode
         *
         * In the mathematical literature, the formula is often written with its well-known form (Euler's formula):
         * @code
         *      signal(x) = exp(j * (2 * M_PI * k * x + phi))
         * @endcode
         *
         * A random noise is added to each formula to distort the signal.
         *
         * @param length The length of the signal. Specifies the number of points in the space domain.
         * @param frequency Represents the spatial frequency, which determines how many cycles occur per unit distance.
         *                  By "unit distance" we mean the distance between two consecutive samples in the spatial grid.
         *                  It can be meters, millimeters, or any other spatial unit.
         * @param phase The phase (shift) of the signal, which moves the waveform along the space axis (x since it's 1D).
         * @param noise The range of the random noise added to each sample of the signal.
         * @return The generated signal (complex signal).
         */
        std::vector<std::complex<double>> generate1DSignal(
            int length, double frequency, double phase, double noise
        ) override;

        /**
         * Generate a random (thanks to noise) one-dimensional real signal in the space domain.
         *
         * Let the spatial frequency be k = frequency.
         *
         * Let the unique spatial coordinate be x (since it's 1D).
         *
         * Let the phase shift be phi.
         *
         * The signal is generated using the formula (real part of the original complex signal):
         * @code
         * signal(x) = cos(2 * M_PI * k * x + phi) + noise
         * @endcode
         *
         * Using the parameters name of the function, the formula becomes:
         * @code
         * signal(x) = cos(2 * M_PI * frequency * x + phase) + noise
         * @endcode
         *
         * A random noise is added to each formula to distort the signal.
         *
         * @param length The length of the signal. Specifies the number of points in the space domain.
         * @param frequency Represents the spatial frequency, which determines how many cycles occur per unit distance.
         *         By "unit distance" we mean the distance between two consecutive samples in the spatial grid.
         *         It can be meters, millimeters, or any other spatial unit.
         * @param phase The phase (shift) of the signal, which moves the waveform along the space axis (x since it's 1D).
         * @param noise The range of the random noise added to each sample of the signal.
         * @return The generated real signal (vector of double).
         */
        std::vector<double> generateReal1DSignal( int length, double frequency, double phase, double noise) override;
    };
}

#endif //SPACE_DOMAIN_SIGNAL_GENERATOR_HPP
