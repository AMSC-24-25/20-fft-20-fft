#include "signal-saver/csv-signal-saver.hpp"
#include "utils/timestamp.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

void CsvSignalSaver::saveToFile(const std::vector<std::complex<double>> &signal, const std::string &filePath) const {
    std::ostringstream stringStream;
    stringStream << filePath << "_" << createReadableTimestamp("%Y%m%d_%H%M%S") << "." << getExtension();
    const std::string output_file_path = stringStream.str();
    printf("Saving signal to file: %s\n", output_file_path.c_str());
    // validate the input
    if (signal.empty()) {
        throw std::invalid_argument("Signal cannot be empty");
    }
    if (filePath.empty()) {
        throw std::invalid_argument("File path cannot be empty");
    }
    // ensure the parent directory exists
    std::filesystem::path path(filePath);
    if (path.has_extension()) {
        throw std::invalid_argument("File path should not contain an extension: " + filePath);
    }
    create_directories(path.parent_path());
    // check if the file already exists
    if (std::filesystem::exists(output_file_path)) {
        throw std::runtime_error("File already exists: " + output_file_path);
    }
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

[[nodiscard]] const std::string &CsvSignalSaver::getExtension() const {
    static const std::string extension = "csv";
    return extension;
}
