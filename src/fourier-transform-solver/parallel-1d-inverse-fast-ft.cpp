#include "fourier-transform-solver/parallel-1d-inverse-fast-ft.hpp"
#include "utils/bit-reversal.hpp"
#include <chrono>


void Parallel1DInverseFastFT::compute() {
    /**
     * Parallel Cooley-Tukey Inverse Fast Fourier Transform (FFT) Algorithm.
     * The algorithm is intentionally uncommented because it is well commented in the sequential version.
     * Therefore, if you are looking for a well-commented version, please check the sequential version.
     */
    printf("Running the Parallel 1D Inverse Fast Fourier Transform solver...\n");
    // 0. Prepare the variables.
    const size_t N = input.size();
    const auto log2N = static_cast<size_t>(std::log2(N));
    // note: the angle is positive because it is the inverse FFT
    //       it is related to the complex exponential used to compute the IFFT;
    constexpr double angle = 2 * M_PI;
    // save time before starting the computation (performance measurement)
    const std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::high_resolution_clock::now();

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

    /**
     * 3. Parallel Normalization.
     * when performing the IFFT, the transformation back to the domain introduces
     * a scaling factor of N (number of points);
     * the mathematical IFFT formula includes a division by N to correctly scale
     * the transformed data back to its original amplitude.
     * we use OpenMP to parallelize the normalization step.
     */
#pragma omp parallel for
        for (size_t i = 0; i < N; ++i) {
            input[i] /= static_cast<double>(N);
        }

    // save time after the computation is done (performance measurement)
    const std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::high_resolution_clock::now();

    // save the solution to the output vector
    solution = input;

    printf("Parallel 1D Inverse Fast Fourier Transform computed. Elapsed time: %f ms.\n",
        std::chrono::duration<float, std::milli>(end_time - start_time).count());
}
