#ifndef JSON_CONFIGURATION_LOADER_HPP
#define JSON_CONFIGURATION_LOADER_HPP

#include "nlohmann/json.hpp"

#include "json_field_handler.hpp"
#include "config_loader/base_configuration_loader.hpp"

/**
 * JSONConfigurationLoader is a concrete class that implements the BaseConfigurationLoader interface.
 */
class JSONConfigurationLoader final : public BaseConfigurationLoader {
    std::optional<JsonFieldHandler> configurationData;

public:
    /**
     * Get the configuration data that was loaded.
     * @return The configuration data as a JsonFieldHandler object.
     * @throw std::runtime_error If no configuration data was loaded.
     */
    [[nodiscard]] const JsonFieldHandler &getConfigurationData() const;

    /**
     * Given a file path to a JSON file, load the configuration from the file.
     * @param filePath The path to the configuration file.
     * @throw std::runtime_error If the file is not a JSON file; the file cannot be opened;
                                 the file is empty; the file is not in JSON format (after loading).
     */
    void loadConfigurationFromFile(const std::string &filePath) override;
};

#endif //JSON_CONFIGURATION_LOADER_HPP
