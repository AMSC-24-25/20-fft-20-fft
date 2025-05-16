#ifndef BASE_SIGNAL_SAVER_HPP
#define BASE_SIGNAL_SAVER_HPP

#include <vector>
#include <complex>
#include <string>

/**
 * Abstract class for saving signals to a file.
 *
 * It is usually useful to export the signal.
 */
class BaseFileSignalSaver {
public:
    virtual ~BaseFileSignalSaver() = default;

    /**
     * Save the signal to a file. The format of the file is defined by the implementation.
     * For example, the CsvSignalSaver saves the signal to a CSV file,
     * the BinarySignalSaver saves the signal to a binary file, etc.
     * @param signal The signal to save.
     * @param filePath The file path where the signal will be saved.
     */
    virtual void saveToFile(const std::vector<std::complex<double>>& signal, const std::string& filePath) const = 0;

    /**
     * Get the extension of the file.
     * @return The extension of the file.
     */
    [[nodiscard]] virtual const std::string &getExtension() const = 0;
};


#endif //BASE_SIGNAL_SAVER_HPP
