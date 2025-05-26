#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <chrono>

#include "handlers/signal_saver/csv_signal_saver.hpp"
#include "utils/timestamp.hpp"

namespace sp::saver
{
#ifdef _WIN32 || _WIN64
    const char SEP = '\\';
#else
    constexpr char SEP = '/';
#endif

    void CsvSignalSaver::saveToFile(
        const std::vector<std::complex<double>> &signal,
        const std::string &filePath
    ) const {
        std::ostringstream stringStream;
        stringStream << filePath << "_"
                     << utils::timestamp::createReadableTimestamp("%Y%m%d_%H%M%S")
                     << "." << getExtension();
        const std::string output_file_path = stringStream.str();
        // validate the input
        if (signal.empty()) {
            throw std::invalid_argument("Signal cannot be empty");
        }
        if (filePath.empty()) {
            throw std::invalid_argument("File path cannot be empty");
        }
        // ensure the parent directory exists (OS-independent separator)
        size_t found = filePath.find_last_of(SEP);
        std::string parent_dir = (found != std::string::npos) ? filePath.substr(0, found) : ".";
        struct stat info;
        if (stat(parent_dir.c_str(), &info) != 0) {
            // Directory does not exist, create it
            if (mkdir(parent_dir.c_str(), 0777) != 0) {
                throw std::runtime_error("Failed to create directory: " + parent_dir);
            }
        }
        // Check if file exists
        /*
        std::ifstream file(output_file_path);
        if (file.good()) {
            throw std::runtime_error("File already exists: " + output_file_path);
        }
        file.close();
        */
        // define the file
        std::ofstream file(output_file_path);
        // check if the file was opened
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + output_file_path);
        }
        // write the signal to the file
        for (const std::complex<double> &sample : signal) {
            file << sample.real() << "," << sample.imag() << "\n";
        }
        // close the file
        file.close();
        printf("Signal saved successfully to file: %s\n", output_file_path.c_str());
    }

    void CsvSignalSaver::saveToFile(const std::vector<double> &signal, const std::string &filePath) const {
        std::ostringstream stringStream;
        stringStream << filePath << "_"
                     << utils::timestamp::createReadableTimestamp("%Y%m%d_%H%M%S") << "." << getExtension();
        const std::string output_file_path = stringStream.str();
        printf("Saving signal to file: %s\n", output_file_path.c_str());
        // validate the input
        if (signal.empty()) {
            throw std::invalid_argument("Signal cannot be empty");
        }
        if (filePath.empty()) {
            throw std::invalid_argument("File path cannot be empty");
        }
        // ensure the parent directory exists (OS-independent separator)
        size_t found = filePath.find_last_of(SEP);
        std::string parent_dir = (found != std::string::npos) ? filePath.substr(0, found) : ".";
        struct stat info;
        if (stat(parent_dir.c_str(), &info) != 0) {
            // Directory does not exist, create it
            if (mkdir(parent_dir.c_str(), 0777) != 0) {
                throw std::runtime_error("Failed to create directory: " + parent_dir);
            }
        }
        // check if file exists
        /*
        std::ifstream file(output_file_path);
        if (file.good()) {
            throw std::runtime_error("File already exists: " + output_file_path);
        }
        file.close();
        */
        // define the file
        std::ofstream file(output_file_path);
        // check if the file was opened
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + output_file_path);
        }
        // write the signal to the file
        for (const double &sample : signal) {
            file << sample << "\n";
        }
        // close the file
        file.close();
        printf("Signal saved successfully to file: %s\n", output_file_path.c_str());
    }

    [[nodiscard]] const std::string &CsvSignalSaver::getExtension() const {
        static const std::string extension = "csv";
        return extension;
    }
}
