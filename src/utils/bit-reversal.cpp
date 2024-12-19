#include "utils/bit-reversal.hpp"

std::vector<std::complex<double>> & sequentialBitReversal(std::vector<std::complex<double>> &data) {
    const size_t N = data.size();
    const int log_N = static_cast<int>(std::log2(N));
    for (size_t i = 0; i < N; ++i) {
        size_t reversed_index = 0;
        for (int j = 0; j < log_N; ++j) {
            if (i & (1 << j)) {
                reversed_index |= (1 << (log_N - j - 1));
            }
        }
        if (reversed_index > i) {
            std::swap(data[i], data[reversed_index]);
        }
    }
    return data;
}

std::vector<std::complex<double>> & parallelBitReversal(std::vector<std::complex<double>> &data) {
    const size_t N = data.size();
    const int log_N = static_cast<int>(std::log2(N));
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        int reversed_index = 0;
        for (int j = 0; j < log_N; ++j) {
            if (i & (1 << j)) {
                reversed_index |= (1 << (log_N - j - 1));
            }
        }
        if (reversed_index > i) {
            std::swap(data[i], data[reversed_index]);
        }
    }
    return data;
}
