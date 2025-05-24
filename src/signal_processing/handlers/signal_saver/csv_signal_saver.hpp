#ifndef CSV_SIGNAL_SAVER_HPP
#define CSV_SIGNAL_SAVER_HPP
#include "base_file_signal_saver.hpp"

namespace sp::saver
{
    class CsvSignalSaver final : public BaseFileSignalSaver {
    public:
        /**
         * Save the signal to a CSV file.
         *
         * The file is saved in the following format:
         * real1,imag1
         * real2,imag2
         * ...
         * So the real and imaginary parts of each sample are separated by a comma.
         *
         * The output filename will be the given filePath with the current timestamp and the csv extension.
         * So if the filePath is "output/signal", the output file will be "output/signal_20241201_123456.csv":
         * - "output/signal" is the filePath
         * - "20241201" is the date in the format YYYYMMDD (year, month, day)
         * - "123456" is the time in the format HHMMSS (hour, minute, second)
         * @param signal The signal to save.
         * @param filePath The file path where the signal will be saved. It shouldn't contain the extension.
         * @throws std::runtime_error if the file can't be opened.
         */
        void saveToFile(const std::vector<std::complex<double>>& signal, const std::string& filePath) const override;

        /**
         * Get the extension of the file created by this class.
         * @return The csv extension.
         */
        [[nodiscard]] const std::string &getExtension() const override;
    };
}

#endif //CSV_SIGNAL_SAVER_HPP
