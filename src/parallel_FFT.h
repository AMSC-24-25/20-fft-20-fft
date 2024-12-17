#include <vector>
#include <complex>
#include <cmath>
#include <omp.h>

class FFT {
protected:
    std::vector<std::complex<double>>& data;
    int N;

public:
    FFT(std::vector<std::complex<double>>& x) : data(x), N(x.size()) {}

    // Function to perform bit-reversal
    void bit_reversal() {
        int log_N = std::log2(N);
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
    }

    // Function to compute the FFT (this will be used by the derived class)
    virtual void compute() {
        // Bit-reversal step
        bit_reversal();

        // Butterfly computation
        for (int size = 2; size <= N; size *= 2) {
            int half_size = size / 2;
            double angle = -2.0 * M_PI / size;

            #pragma omp parallel for
            for (int start = 0; start < N; start += size) {
                for (int i = 0; i < half_size; ++i) {
                    int j = start + i;
                    int k = j + half_size;
                    std::complex<double> t = std::exp(std::complex<double>(0, angle * i)) * data[k];
                    data[k] = data[j] - t;
                    data[j] += t;
                }
            }
        }
    }
};

