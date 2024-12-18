#include <vector>
#include <complex>
#include <cmath>
#include <omp.h>

class IFFT : public FFT {
public:
    IFFT(std::vector<std::complex<double>>& x) : FFT(x) {}

    // Function to compute the IFFT
    void compute() override {
        // Bit-reversal step
        bit_reversal();

        // Butterfly computation for IFFT (angle sign is positive)
        for (int size = 2; size <= N; size *= 2) {
            int half_size = size / 2;
            double angle = 2.0 * M_PI / size;  // Angle sign is positive for IFFT

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

        // Normalize the result (divide by N)
        #pragma omp parallel for
        for (int i = 0; i < N; ++i) {
            data[i] /= N;
        }
    }
};