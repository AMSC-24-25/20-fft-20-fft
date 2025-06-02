#ifndef BASE_CONFIGURATION_LOADER_HPP
#define BASE_CONFIGURATION_LOADER_HPP
#include <string>

/**
 * Configuration module for signal processing.
 *
 * This module provides methods and classes for loading and managing configuration data
 * for signal processing applications.
 */
namespace sp::config
{
    /**
    * BaseConfigurationLoader is an abstract class that defines the interface for loading configuration
    * from a configuration.
    */
    class BaseConfigurationLoader {
    public:
        // BaseConfigurationLoader() = default;
        virtual ~BaseConfigurationLoader() = default;

        /**
         * Given a file path, load the configuration from the file.
         * @param filePath The path to the configuration file.
         */
        virtual void loadConfigurationFromFile(const std::string &filePath) = 0;
    };
}

#endif //BASE_CONFIGURATION_LOADER_HPP
