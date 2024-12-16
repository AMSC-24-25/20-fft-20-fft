#include <iostream>
#include "config-loader/json-configuration-loader.hpp"
#include "signal-generator/space-domain-signal-generator.hpp"
#include "signal-generator/time-domain-signal-generator.hpp"

/**
 * Environment variable name for the file path for the configuration file.
 */
#define ENV_FILE_PATH "CONFIG_FILE_PATH_FFT"

int main() {
    // get the file path from environment variable
    if (getenv(ENV_FILE_PATH) == nullptr) {
        std::cerr << "Warning: Environment variable " << ENV_FILE_PATH << " is not set. "
                                                                          "Using default configuration file path.\n";
        // if the environment variable is not set, use the default configuration file path
        setenv(ENV_FILE_PATH, "resources/sample-config.json", 0);
    }
    const std::string filePath = getenv(ENV_FILE_PATH);

    // load the configuration from the file
    JSONConfigurationLoader loader;
    loader.loadConfigurationFromFile(filePath);
    // get the simulation parameters
    const int signal_length = loader.getSignalLength();
    const int lower_bound = loader.getLowerBound();
    const int upper_bound = loader.getUpperBound();
    const std::string signal_domain = loader.getSignalDomain();
    const std::optional<int> seed = loader.hasSeed() ? std::optional(loader.getSeed()) : std::nullopt;

    // TODO: implement an enum for signal domain values...
    // generate the signal
    std::vector<std::complex<double>> signal;
    if (signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        TimeDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generateSignal(signal_length, lower_bound, upper_bound);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        SpaceDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generateSignal(signal_length, lower_bound, upper_bound);
    }

    // debug: print the generated signal
    printf("Vector generated with %lu elements\n", signal.size());
    for (const auto &s : signal) {
        std::cout << s << std::endl;
    }
    return 0;
}
