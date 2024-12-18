#include <iostream>
#include "config-loader/json-configuration-loader.hpp"
#include "signal-generator/space-domain-signal-generator.hpp"
#include "signal-generator/time-domain-signal-generator.hpp"
#include "signal-saver/csv-signal-saver.hpp"

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
    const JsonFieldHandler json_loaded = loader.getConfigurationData();
    const int signal_length = json_loaded.getSignalLength();
    const double frequency = json_loaded.getHzFrequency();
    const double phase = json_loaded.getPhase();
    const double noise = json_loaded.getNoise();
    const std::string signal_domain = json_loaded.getSignalDomain();
    const std::optional<int> seed = json_loaded.hasSeed() ? std::optional(json_loaded.getSeed()) : std::nullopt;

    // TODO: implement an enum for signal domain values...
    // generate the signal
    std::vector<std::complex<double>> signal;
    if (signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        TimeDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, frequency, phase, noise);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        SpaceDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, frequency, phase, noise);
    }

    // debug: print the generated signal
    printf("Vector generated with %lu elements\n", signal.size());
    for (const auto &s : signal) {
        std::cout << s << std::endl;
    }

    // save the signal to a file
    const std::string output_file_path = "output/signal";
    const CsvSignalSaver csv_signal_saver;
    csv_signal_saver.saveToFile(signal, output_file_path);

    return 0;
}
