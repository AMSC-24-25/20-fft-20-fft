#include <iostream>

#include "handlers/signal_generator/space_domain_signal_generator.hpp"

namespace sp::signal_gen
{
    std::vector<std::complex<double> > SpaceDomainSignalGenerator::generate1DSignal(
        const int length, const double frequency, const double phase, const double noise
    ) {
        // init
        std::vector<std::complex<double>> signal;
        // reserve the space for the signal
        try {
            signal.reserve(length);
        } catch (std::length_error &e) {
            std::cerr << "Error: the length of the signal cannot be grater than " << signal.max_size()
                    << ". Reason: " << e.what() << std::endl;
        }
        // there is memory space for the signal, so generate it
        // initialize the Gaussian distribution for the noise
        std::normal_distribution<> gaussian(0, noise);
        // angular spatial frequency or wave number
        const double angular_spatial_frequency = 2 * M_PI * frequency;
        // generate the signal 1D, so we have only one spatial coordinate called x for convenience
        for (int x = 0; x < length; ++x) {
            signal.emplace_back(
                std::cos(angular_spatial_frequency * x + phase) + gaussian(_engine),
                std::sin(angular_spatial_frequency * x + phase) + gaussian(_engine)
            );
        }
        return signal;
    }

    std::vector<double> SpaceDomainSignalGenerator::generateReal1DSignal(
        const int length, const double frequency, const double phase, const double noise
    ) {
        // init
        std::vector<double> signal;
        // reserve the space for the signal
        try {
            signal.reserve(length);
        } catch (std::length_error &e) {
            std::cerr << "Error: the length of the signal cannot be grater than " << signal.max_size()
                    << ". Reason: " << e.what() << std::endl;
        }
        // there is memory space for the signal, so generate it
        // initialize the Gaussian distribution for the noise
        std::normal_distribution<> gaussian(0, noise);
        // angular spatial frequency or wave number
        const double angular_spatial_frequency = 2 * M_PI * frequency;
        // generate the signal 1D, so we have only one spatial coordinate called x for convenience
        for (int x = 0; x < length; ++x) {
            signal.emplace_back(
                std::cos(angular_spatial_frequency * x + phase) + gaussian(_engine)
            );
        }
        return signal;
    }
}
