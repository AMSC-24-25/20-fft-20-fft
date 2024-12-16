#include <iostream>
#include "config-loader/json-configuration-loader.hpp"

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
        setenv("CONFIG_FILE_PATH_FFT", "resources/sample-config.json", 0);
    }
    const std::string filePath = getenv("CONFIG_FILE_PATH_FFT");
    // load the configuration from the file
    JSONConfigurationLoader loader;
    loader.loadConfigurationFromFile(filePath);

    // TODO: use the configuration to generate the input;
    //       to get the signal length, use loader.getSignalLength();
    //       to get the signal domain, use loader.getSignalDomain();

    return 0;
}
