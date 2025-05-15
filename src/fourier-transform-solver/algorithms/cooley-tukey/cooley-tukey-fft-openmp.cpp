#include "fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft-openmp.hpp"

#include "utils/bit-reversal.hpp"

namespace fft::algorithms::cooley_tukey {
    void computeCooleyTurkeyFFTOpenMP(std::vector<std::complex<double>>& input)
    {
        /**
         * Parallel Cooley-Tukey Fast Fourier Transform (FFT) Algorithm.
         * The algorithm is intentionally uncommented because it is well commented in the sequential version.
         * Therefore, if you are looking for a well-commented version, please check the sequential version.
         */
        // 0. Prepare the variables.
        const size_t N = input.size();
        const auto log2N = static_cast<size_t>(std::log2(N));
        constexpr double angle = -2 * M_PI;

        // 1. Bit-Reversal Permutation
        parallelBitReversal(input);

        // 2. Iterative Cooley-Tukey FFT
        // 2.1. Main loop - Stages: iterate log2(N) times.
        for (size_t s = 1; s <= log2N; ++s) {
            const size_t m = 1 << s;  // 2^s
            const size_t m2 = m >> 1; // m/2
            const std::complex<double> wm = std::polar(1.0, angle / static_cast<double>(m));

            /**
             * Loop Parallelization (OpenMP).
             * each iteration of the loop processes a different chunk of size 'm', allowing parallel execution.
             * therefore, each thread processes a different chunk of the input vector.
             */
#pragma omp parallel for
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
    }
}

