#ifndef JSON_CONFIGURATION_LOADER_HPP
#define JSON_CONFIGURATION_LOADER_HPP

#include <nlohmann/json.hpp>
#include "config-loader/abstract-configuration-loader.hpp"

/**
 * JSONConfigurationLoader is a concrete class that implements the AbstractConfigurationLoader interface.
 */
class JSONConfigurationLoader final : public AbstractConfigurationLoader {
    nlohmann::json configurationData = nlohmann::json::value_t::discarded;

public:
    /**
     * Get the configuration data that was loaded.
     * @return The configuration data.
     * @throw std::runtime_error If the configuration data is empty.
     */
    [[nodiscard]] const nlohmann::json &getConfigurationData() const;

    /**
     * Get the signal domain from the configuration data.
     * @return The signal domain.
     * @throw std::runtime_error If the signal domain is null.
     */
    [[nodiscard]] std::string getSignalDomain() const;

    /**
     * Get the signal length from the configuration data.
     * @return The signal length.
     * @throw std::runtime_error If the signal length is null.
     */
    [[nodiscard]] int getSignalLength() const;

    /**
     * The lower bound for the uniform distribution.
     * @return The lower bound for the uniform distribution.
     */
    [[nodiscard]] int getLowerBound() const;

    /**
     * The upper bound for the uniform distribution.
     * @return The upper bound for the uniform distribution.
     */
    [[nodiscard]] int getUpperBound() const;

    /**
     * Get the seed from the configuration data.
     * @return The seed.
     * @throw std::runtime_error If the seed is null.
     */
    [[nodiscard]] int getSeed() const;

    /**
     * Check if the configuration data has a seed.
     * @return True if the configuration data has a seed; false otherwise.
     */
    [[nodiscard]] bool hasSeed() const;

    /**
     * Given a file path to a JSON file, load the configuration from the file.
     * @param filePath The path to the configuration file.
     * @throw std::runtime_error If the file is not a JSON file; the file cannot be opened;
                                 the file is empty; the file is not in JSON format (after loading).
     */
    void loadConfigurationFromFile(const std::string &filePath) override;
};

#endif //JSON_CONFIGURATION_LOADER_HPP
