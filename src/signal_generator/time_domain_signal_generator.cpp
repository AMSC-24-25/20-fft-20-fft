#include <iostream>

#include "signal_generator/time_domain_signal_generator.hpp"

std::vector<std::complex<double> > TimeDomainSignalGenerator::generate1DSignal(
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
    const double angular_frequency = 2 * M_PI * frequency;
    // generate the signal
    for (int time = 0; time < length; ++time) {
        signal.emplace_back(
            std::cos(angular_frequency * time + phase) + gaussian(_engine),
            std::sin(angular_frequency * time + phase) + gaussian(_engine)
        );
    }
    return signal;
}
