#include <utils/timestamp.hpp>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>

std::string createReadableTimestamp(const std::string &format) {
    // get the timestamp
    const time_t now = std::time(nullptr);
    // get the local timezone using the timestamp
    const std::tm tm = *std::localtime(&now);
    // create the timestamp string
    std::ostringstream oss;
    // use the format provided by the user
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}