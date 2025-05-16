#ifndef ZIGZAG_SCAN_HPP
#define ZIGZAG_SCAN_HPP
#include <vector>

/**
 * Zigzag Scan Algorithm.
 *
 * This module provides functions to perform zigzag scanning on a 2D matrix
 * and to reconstruct the original matrix from the zigzag-scanned vector.
 */
namespace signal_processing::utils::zigzag_scan
{
    class ZigZagScan {
    public:
        /**
         * Function that applies a zigzag scan on a 2D matrix.
         * Converts a 2D matrix into a 1D vector following the zigzag traversal pattern.
         * @param matrix: 2D input matrix to scan.
         * @return 1D vector of zigzag-scanned values.
         */
        static const std::vector<double> scan(const std::vector<std::vector<double>>& matrix);

        /**
         * Function that applies the inverse zigzag scan to a 1D vector.
         * Reconstructs a 2D matrix from a zigzag-scanned 1D vector.
         * @param scanned: 1D vector of zigzag-scanned values;
         * @param rows: number of rows in the target matrix;
         * @param cols: number of columns in the target matrix.
         * @return 2D matrix reconstructed from the zigzag-scanned vector.
         */
        static const std::vector<std::vector<double>> inverse_scan(
            const std::vector<double>& scanned,
            const int rows,
            const int cols
        );
    };
}

#endif //ZIGZAG_SCAN_HPP