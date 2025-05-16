#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP
#include <string>

/**
 * Naive timestamp handler.
 */
namespace signal_processing::utils::timestamp
{
    /**
    * Create a timestamp string with the format provided.
    * @param format The timestamp format.
    *               See the documentation for more information:
    *               <a href="https://en.cppreference.com/w/cpp/io/manip/put_time">put_time function</a>
    * @return The timestamp string with the format provided.
    */
    std::string createReadableTimestamp(const std::string &format);
}

#endif //TIMESTAMP_HPP
