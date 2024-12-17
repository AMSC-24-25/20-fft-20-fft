#ifndef JSON_FIELD_HANDLER_HPP
#define JSON_FIELD_HANDLER_HPP

#include <string>
#include <unordered_map>

/**
 * JsonFieldHandler class is a utility class that provides field names for the JSON configuration file.
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

private:
  /**
   * Unordered map that maps the field enum to the field name.
   */
  static const std::unordered_map<Field, std::string> fieldNames;
};

#endif //JSON_FIELD_HANDLER_HPP
