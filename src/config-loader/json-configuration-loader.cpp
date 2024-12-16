/**
 * JSONConfigurationLoader class implementation.
 */

#include <fstream>
#include <config-loader/json-configuration-loader.hpp>

using json = nlohmann::json;

const nlohmann::json &JSONConfigurationLoader::getConfigurationData() const {
    if (configurationData.is_discarded()) {
        throw std::runtime_error("Configuration data is empty.");
    }
    return configurationData;
}

std::string JSONConfigurationLoader::getSignalDomain() const {
    if (configurationData.at("signal_domain").is_null()) {
        throw std::runtime_error("Signal domain is null.");
    }
    return configurationData.at("signal_domain");
}

int JSONConfigurationLoader::getSignalLength() const {
    if (configurationData.at("signal_length").is_null()) {
        throw std::runtime_error("Signal length is null.");
    }
    return configurationData.at("signal_length");
}

int JSONConfigurationLoader::getSeed() const {
    if (configurationData.at("seed").is_null()) {
        throw std::runtime_error("Seed is null.");
    }
    return configurationData.at("seed");
}

bool JSONConfigurationLoader::hasSeed() const {
    return configurationData.contains("seed");
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
    try {
        fileStream >> configurationData;
    } catch (const nlohmann::json::parse_error &e) {
        throw std::runtime_error("Failed to parse JSON file: " + filePath + "; Parse error: " + e.what());
    }
    // print the actual configuration data
    printf("The configuration data has been loaded from the file: %s.\n%s\n\n",
        filePath.c_str(), configurationData.dump(4).c_str());
}
