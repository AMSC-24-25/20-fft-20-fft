#ifndef CONFIGURATION_LOADER_HPP
#define CONFIGURATION_LOADER_HPP
#include <string>

/**
 * AbstractConfigurationLoader is an abstract class that defines the interface for loading configuration
 * from a configuration.
 */
class AbstractConfigurationLoader {
public:
    // AbstractConfigurationLoader() = default;
    virtual ~AbstractConfigurationLoader() = default;

    /**
     * Given a file path, load the configuration from the file.
     * @param filePath The path to the configuration file.
     */
    virtual void loadConfigurationFromFile(const std::string &filePath) = 0;
};

#endif //CONFIGURATION_LOADER_HPP
