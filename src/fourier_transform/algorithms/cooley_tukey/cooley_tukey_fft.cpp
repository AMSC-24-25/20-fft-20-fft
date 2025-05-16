#include "fourier_transform/algorithms/cooley_tukey/cooley_tukey_fft.hpp"

#include "utils/bit_reversal.hpp"

namespace fft::algorithms::cooley_tukey {
    void computeCooleyTurkeyFFT(std::vector<std::complex<double>>& input) {
        /**
         * Sequential Cooley-Tukey Fast Fourier Transform (FFT) Algorithm.
         * The algorithm is well commented in the code below.
         * If you are looking for an uncommented version, please check the parallel version.
         */
        // 0. Prepare the variables.
        // - get the size of the input vector
        const size_t N = input.size();

        /**
         * - get the log2(N) value of the input vector,
         *   this because the algorithm iterates log2(N) times;
         *   because the divide-and-conquer algorithm splits the input vector
         *   in half at each iteration until it reaches the base case of 1 element
         */
        const auto log2N = static_cast<size_t>(std::log2(N));

        /**
         * - the angle used in the algorithm;
         *   it is related to the complex exponential used to compute the FFT;
         *   since we need to compute the twiddle factors,
         *   we can precompute the angle value and "cache" it;
         *   the negative sign indicates the direction of the rotation in the complex plane,
         *   aligning with the definition of the DFT (Discrete Fourier Transform);
         *   remark: the twiddle factor is the exponential term in the DFT formula,
         *           and it represents a complex exponential term that "twiddles" or manipulates
         *           the phase of the input signals during the FFT computation;
         *           twiddle factor is: e^(-2 * pi * i * k * n / N), where:
         *           - i is the imaginary unit,
         *           - k is the current output index,
         *           - n is the input index,
         *           - N is the total number of points.
         */
        constexpr double angle = -2 * M_PI;

        // 1. Bit-Reversal Permutation.
        sequentialBitReversal(input);

        /**
         * 2. Iterative Cooley-Tukey FFT.
         * 2.1. Main loop - Stages: iterate log2(N) times.
         *      iterate over the stages of the FFT computation (log2(N) stages);
         *      each stage processes sub-problems of increasing size,
         *      combining smaller DFT results into larger ones.
         */
        for (size_t s = 1; s <= log2N; ++s) {
            /**
             * Stage size value.
             * the size of the sub-problems in the current stage of the FFT;
             * we use the bitwise shift operator (<<) to compute the power of 2,
             * which is a faster operation than pow(2, s); for example, 1 << 3 = 8, which is 2^3;
             * the size of the subproblems is 2^s, and it is pretty clear that the subproblems are doubled at each stage;
             * this is an interesting observation because it is a divide-and-conquer algorithm (Cooley-Tukey);
             */
            const size_t m = 1 << s;

            /**
             * Midpoint value.
             * half the size of the sub-problems in the current stage of the FFT;
             * it is calculated using the bitwise shift operator (>>) to divide the size by 2,
             * another faster operation than m / 2; for example, 8 >> 1 = 4, which is 8 / 2;
             * it is used to determine the midpoint of the current sub-problems;
             * since the Cooley-Tukey algorithm combines elements from the first and second halves
             * of the subproblems during the butterfly calculation (see below),
             * knowing the midpoint (m2) helps to pair these elements correctly and
             * apply the twist factor correctly;
             */
            const size_t m2 = m >> 1;

            /**
             * Stage Twiddle factor.
             * precompute the twiddle factor for the current stage
             * (the angle is divided by the current stage size)
             * since the twiddle factor is a complex exponential,
             * we can represent it in polar form (magnitude and angle);
             * the magnitude is 1.0 (unit circle), and the angle is the angle value divided by the current stage size;
             * before entering the inner loop, the twiddle factor is initialized to 1.0 because there is no rotation yet.
             * see the w *= wm operation in the inner loop for the usage of the twiddle factor
             */
            const std::complex<double> wm = std::polar(1.0, angle / static_cast<double>(m));

            /**
             * 2.2. Inner loop - Iterating over the 'sub-DFTs' in the current stage.
             *      the 'sub-DFTs' are computed in place and not recursively!
             *      the iteration is over the sub-DFTs in the current stage, processing each sub-DFT independently;
             *      the boundary of the iteration is the size of the input vector (N), so processing all the elements;
             *      the iteration step is the size of the current stage (m), i.e. jumping over the sub-DFTs;
             *      note that the step size m represents the current size of the sub-DFTs being processed in this stage;
             */
            for (size_t k = 0; k < N; k += m) {
                /**
                 * Incremental Twiddle factor.
                 * it is initialized to 1.0 (magnitude) and a phase angle of 0 (no rotation);
                 * it is used within the inner loop to apply the current twiddle factor
                 * to the elements being processed;
                 * it starts with no rotation, and it is updated at each iteration of the inner loop
                 * to apply the rotation (phase shift);
                 */
                std::complex<double> w{1.0, 0};

                /**
                 * 2.3. Butterfly computation.
                 *      the butterfly computation is the core of the Cooley-Tukey algorithm;
                 *      at each iteration, the algorithm performs a butterfly operation on a pair of elements within the sub-DFT;
                 *      this loop handles the combination of elements with the appropriate twiddle factor;
                 *      it continues until the midpoint of the sub-DFT is reached:
                 *      - it avoids processing the same pair of elements twice (redundancy);
                 *      - it processes the first half of the sub-DFT and combines it with the second half;
                 */
                for (size_t j = 0; j < m2; ++j) {
                    /**
                     * Phase Adjustment.
                     * it represents the phase adjustment element from the second half of the current chunk;
                     * it is calculated by multiplying the incremental twiddle factor with the element from the second half;
                     *  - for a chunk starting at index k, the first half spans indeces [k, k + m2 - 1];
                     *  - therefore, the second half spans indeces [k + m2, k + m - 1];
                     * using m2, to access an element in the second half of the chunk, we use the index k + j + m2;
                     * it is called phase adjustment because it refers to changing the phase of an element
                     * by multiplying it by the twiddle factor
                     * (it increases the mapping from data to frequency domain);
                     */
                    std::complex<double> t = w * input[k + j + m2];
                    /**
                     * First Half element.
                     */
                    std::complex<double> u = input[k + j];
                    /**
                     * Butterfly Operation.
                     * computation of the first half (k+j) and second half (k+j+m2) of the butterfly operation;
                     * the operations ensure that the elements are combined correctly and that the entire
                     * frequency spectrum of the input signal is captured;
                     */
                    input[k + j] = u + t;
                    input[k + j + m2] = u - t;
                    /**
                     * Twiddle Factor Update.
                     * the twiddle factor is applied during each stage of the FFT computation;
                     */
                    w *= wm;
                }
            }
        }
    }
}