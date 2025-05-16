#ifndef ZIGZAG_SCAN_HPP
#define ZIGZAG_SCAN_HPP
#include <vector>

class ZigZagScan {
public:
    static const std::vector<double> scan(const std::vector<std::vector<double>>& matrix);
    static const std::vector<std::vector<double>> inverse_scan(const std::vector<double>& scanned, const int rows, const int cols);
};

#endif //ZIGZAG_SCAN_HPP