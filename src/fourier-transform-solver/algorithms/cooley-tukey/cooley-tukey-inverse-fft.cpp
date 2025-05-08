#include "cooley-tukey-fft.hpp"


#include "utils/bit-reversal.hpp"

namespace fft::algorithms::cooley_tukey {
    void computeCooleyTurkeyInverseFFT(std::vector<std::complex<double>>& input) {
        /**
         * Sequential Cooley-Tukey Inverse Fast Fourier Transform (FFT) Algorithm.
         * The algorithm is intentionally uncommented because it is well commented in the sequential version (FFT).
         * Therefore, if you are looking for a well-commented version, please check the sequential version (FFT).
         */
        printf("Running the Sequential 1D Inverse Fast Fourier Transform solver...\n");
        // 0. Prepare the variables.
        const size_t N = input.size();
        const auto log2N = static_cast<size_t>(std::log2(N));
        // note: the angle is positive because it is the inverse FFT
        //       it is related to the complex exponential used to compute the IFFT;
        constexpr double angle = 2 * M_PI;

        // 1. Bit-Reversal Permutation
        sequentialBitReversal(input);

        // 2. Iterative Cooley-Tukey FFT
        // 2.1. Main loop - Stages: iterate log2(N) times.
        for (size_t s = 1; s <= log2N; ++s) {
            const size_t m = 1 << s;  // 2^s
            const size_t m2 = m >> 1; // m/2
            const std::complex<double> wm = std::polar(1.0, angle / static_cast<double>(m));

            // 2.2. Inner loop - Iterating over the 'sub-DFTs' in the current stage.
            for (size_t k = 0; k < N; k += m) {
                std::complex<double> w{1.0, 0};
                // 2.3. Butterfly computation.
                for (size_t j = 0; j < m2; ++j) {
                    std::complex<double> t = w * input[k + j + m2];
                    std::complex<double> u = input[k + j];
                    input[k + j] = u + t;
                    input[k + j + m2] = u - t;
                    w *= wm;
                }
            }
        }

        for (size_t i = 0; i < N; ++i) {
            input[i] /= static_cast<double>(N);
        }
    }
}
