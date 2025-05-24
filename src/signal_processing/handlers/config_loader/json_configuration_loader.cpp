/**
 * JSONConfigurationLoader class implementation.
 */

#include <fstream>

#include "handlers/config_loader/json_configuration_loader.hpp"
#include "handlers/config_loader/json_field_handler.hpp"

using json = nlohmann::json;

namespace sp::config
{
    const JsonFieldHandler &JSONConfigurationLoader::getConfigurationData() const {
        if (configurationData.has_value()) {
            return configurationData.value();
        }
        throw std::runtime_error("No JSON configuration data loaded.");
    }

    void JSONConfigurationLoader::loadConfigurationFromFile(const std::string &filePath) {
        // assert that the file have a .json extension
        if (!filePath.ends_with(".json")) {
            throw std::runtime_error("File is not a JSON file: " + filePath);
        }
        // get the file stream
        std::ifstream fileStream(filePath);
        // check if the file stream is open
        if (!fileStream.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }
        // check if the file is empty
        if (fileStream.peek() == std::ifstream::traits_type::eof()) {
            throw std::runtime_error("File is empty: " + filePath);
        }
        // if all checks pass, load the configuration data
        nlohmann::json jsonConfigurationData;
        try {
            fileStream >> jsonConfigurationData;
        } catch (const nlohmann::json::parse_error &e) {
            throw std::runtime_error("Failed to parse JSON file: " + filePath + "; Parse error: " + e.what());
        }
        // create the field handler object
        configurationData = JsonFieldHandler(jsonConfigurationData);
        // close the file stream
        fileStream.close();
        // print the actual configuration data;
        printf("The configuration data has been loaded from the file: %s.\n%s\n\n",
            filePath.c_str(), configurationData->getConfigurationLoaded().dump(4).c_str());
    }
}
