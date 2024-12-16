#include "signal-generator/space-domain-signal-generator.hpp"

#include <iostream>

std::vector<std::complex<double> > SpaceDomainSignalGenerator::generateSignal(
    const int length, const int lower_bound, const int upper_bound
) {
    // the method should be take rows, cols and major order as parameters...
    // but this is a simple implementation of a 1D signal, so the signal is generated as a 1D signal
    std::vector<std::complex<double>> signal;
    // initialize the distributions
    std::uniform_real_distribution real_distribution(static_cast<float>(lower_bound), static_cast<float>(upper_bound));
    std::uniform_real_distribution i_distribution(static_cast<float>(lower_bound), static_cast<float>(upper_bound));
    // reserve the space for the signal
    try {
        signal.reserve(length);
    } catch (std::length_error &e) {
        std::cerr << "Error: the length of the signal cannot be grater than " << signal.max_size()
                  << ". Reason: " << e.what() << std::endl;
    }
    // generate the signal
    for (int i = 0; i < length; ++i) {
        signal.emplace_back(real_distribution(_engine), i_distribution(_engine));
    }
    return signal;
}
