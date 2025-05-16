#ifndef JSON_FIELD_HANDLER_HPP
#define JSON_FIELD_HANDLER_HPP

#include <string>
#include <unordered_map>
#include <utility>

#include "nlohmann/json.hpp"

namespace signal_processing::config
{
   /**
   * JsonFieldHandler class is a utility class that provides field names for the JSON configuration file.
   *
   * It also provides methods to retrieve the field values from the JSON configuration file.
   *
   * It is a kind of wrapper around the JSON object of the nlohmann::json library.
   */
  class JsonFieldHandler {
  public:
    /**
     * Enum class for the field names in the JSON configuration file.
     */
    enum class Field {
      SignalDomain,
      SignalLength,
      HzFrequency,
      Phase,
      Noise,
      Seed
    };

    /**
     * Returns true if the field is a valid field in the JSON configuration file.
     * @param field The field name as a string.
     * @return True if the field is valid, false otherwise.
     */
    static bool hasField(const std::string &field);

    /**
     * Get the field name from the field enum.
     * @param field The field enum.
     * @return The field name.
     */
    static std::string getFieldName(Field field);

    /**
     * Constructor that takes a JSON configuration data.
     * @param json The JSON configuration data.
     * @throw std::runtime_error If the JSON cannot be parsed.
     * @throw std::invalid_argument If the JSON is not valid or the required fields are missing.
     */
    explicit JsonFieldHandler(nlohmann::json json) : configurationLoaded(std::move(json)) {
      // validate the configuration data, throw an exception if it is not valid
      validation();
    }
    // use std::move to move the json object to the configurationLoaded field
    // to avoid copying (or deep copying) the json object

    /**
     * Get the configuration data that was loaded as a JSON object.
     * Not modifiable and not recommended. Use the getter methods instead.
     * @return The configuration data as a JSON object.
     */
    [[nodiscard]] const nlohmann::json &getConfigurationLoaded() const;

    /**
     * Get the signal domain fields from the configuration data.
     * @return The signal domain.
     * @throw std::runtime_error If the signal domain is null.
     */
    [[nodiscard]] std::string getSignalDomain() const;

    /**
     * Get the signal length fields from the configuration data.
     * @return The signal length.
     * @throw std::runtime_error If the signal length is null.
     */
    [[nodiscard]] int getSignalLength() const;

    /**
     * Get the Hz frequency fields from the configuration data.
     * @return The Hz frequency.
     */
    [[nodiscard]] double getHzFrequency() const;

    /**
     * Get the phase fields from the configuration data.
     * @return The phase.
     */
    [[nodiscard]] double getPhase() const;

    /**
     * Get the noise fields from the configuration data.
     * @return The noise.
     */
    [[nodiscard]] double getNoise() const;

    /**
     * Get the seed fields from the configuration data.
     * @return The seed.
     * @throw std::runtime_error If the seed is null.
     */
    [[nodiscard]] int getSeed() const;

    /**
     * Check if the configuration data has a seed. Because the seed is optional, it may not be present.
     * @return True if the configuration data has a seed; false otherwise.
     */
    [[nodiscard]] bool hasSeed() const;

  private:
    /**
     * Unordered map that maps the field enum to the field name.
     */
    static const std::unordered_map<Field, std::string> fieldNames;
    nlohmann::json configurationLoaded;

    /**
     * Validate the configuration data.
     *
     * TODO: The json schema should be used to validate the configuration data.
     * @throw std::invalid_argument If the configuration data is not valid or the required fields are missing.
     */
    void validation() const;
  };
}

#endif //JSON_FIELD_HANDLER_HPP
