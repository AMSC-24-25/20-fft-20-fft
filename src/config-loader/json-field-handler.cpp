#include "config-loader/json-field-handler.hpp"

#include <stdexcept>
#include <bits/ranges_algo.h>

const std::unordered_map<JsonFieldHandler::Field, std::string> JsonFieldHandler::fieldNames = {
    {Field::SignalDomain, "signal_domain"},
    {Field::SignalLength, "signal_length"},
    {Field::HzFrequency, "hz_frequency"},
    {Field::Phase, "phase"},
    {Field::Noise, "noise"},
    {Field::Seed, "seed"}
};

bool JsonFieldHandler::hasField(const std::string &field) {
    /**
     * Find at least one element in the range that satisfies the predicate (lambda function):
     * the value of the pair is equal to the field.
     * - range: fieldNames is an unordered_map,
     *          so it uses std::ranges::begin(fieldNames) and std::ranges::end(fieldNames)
     * - pred: lambda function that checks whether the second element (the value)
     *         of the pair is equal to the field
     */
    return std::ranges::any_of(fieldNames, [&field](const std::pair<Field, std::string> &f) -> bool {
        return f.second == field;
    });
}

std::string JsonFieldHandler::getFieldName(const Field field) {
    // type: std::unordered_map<Field, std::string>::const_iterator
    // note: find() returns an iterator to the element if found,
    //       otherwise it returns an iterator to the end of the container;
    //       it is a sentinel value
    if (const auto field_name = fieldNames.find(field); field_name != fieldNames.end()) {
        return field_name->second;
    }
    throw std::invalid_argument("Field not found");
}

const nlohmann::json &JsonFieldHandler::getConfigurationLoaded() const {
    return configurationLoaded;
}

std::string JsonFieldHandler::getSignalDomain() const {
    const std::string field_name = getFieldName(Field::SignalDomain);
    if (configurationLoaded.at(field_name).is_null()) {
        throw std::runtime_error(field_name + " is null.");
    }
    return configurationLoaded.at(field_name);
}

int JsonFieldHandler::getSignalLength() const {
    const std::string field_name = getFieldName(Field::SignalLength);
    if (configurationLoaded.at(field_name).is_null()) {
        throw std::runtime_error(field_name + " is null.");
    }
    return configurationLoaded.at(field_name);
}

double JsonFieldHandler::getHzFrequency() const {
    const std::string field_name = getFieldName(Field::HzFrequency);
    if (configurationLoaded.at(field_name).is_null()) {
        throw std::runtime_error(field_name + " is null.");
    }
    return configurationLoaded.at(field_name);
}

double JsonFieldHandler::getPhase() const {
    const std::string field_name = getFieldName(Field::Phase);
    if (configurationLoaded.at(field_name).is_null()) {
        throw std::runtime_error(field_name + " is null.");
    }
    return configurationLoaded.at(field_name);
}

double JsonFieldHandler::getNoise() const {
    const std::string field_name = getFieldName(Field::Noise);
    if (configurationLoaded.at(field_name).is_null()) {
        throw std::runtime_error(field_name + " is null.");
    }
    return configurationLoaded.at(field_name);
}


int JsonFieldHandler::getSeed() const {
    const std::string field_name = getFieldName(Field::Seed);
    if (configurationLoaded.at(field_name).is_null()) {
        throw std::runtime_error(field_name + " is null.");
    }
    return configurationLoaded.at(field_name);
}

bool JsonFieldHandler::hasSeed() const {
    return configurationLoaded.contains(getFieldName(Field::Seed));
}
