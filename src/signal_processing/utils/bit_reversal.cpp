#include "utils/bit_reversal.hpp"

namespace signal_processing::utils::bit_reversal
{
     std::vector<std::complex<double>> & sequentialBitReversal(std::vector<std::complex<double>> &data) {
        // size of the input data
        const size_t N = data.size();

        // logaritmic value of the size of the input data,
        // it indicates the number of bits needed to represent the indices in N
        // e.g. N = 8, log_N = 3, because 2^3 = 8
        const size_t log_N = std::log2(N);

        // for each element at index i,
        // calculate the reversed index which is the bit-reversed counterpart of i
        for (size_t i = 0; i < N; ++i) {

            // reversed index
            size_t reversed_index = 0;

            // for each bit in the index i
            for (size_t j = 0; j < log_N; ++j) {

                /**
                 * if condition is true iff i & (1 << j) is not zero:
                 *  - 1 << j shifts the number 1 left by j positions
                 *    (e.g. 1 << 1 = 2 (10), 1 << 2 = 4 (100), 1 << 3 = 8 (1000))
                 *  - i & (1 << j) bitwise AND operator to check if the j-th bit in i is set (1)
                 * example:
                 * - i = 5 (101)
                 *      - j = 0: 5 & (1 << 0) = 101 & 001 = 001 (not equal to zero, so true)
                 *      - j = 1: 5 & (1 << 1) = 101 & 010 = 000 (equal to zero, so false)
                 *      - j = 2: 5 & (1 << 2) = 101 & 100 = 100 (not equal to zero, so true)
                 */
                if (i & (1 << j)) {
                    /**
                     * if the j-th bit in i is set (1), then:
                     *  - use |= as a bitwise OR operator (OR logic gate)
                     *  - shift the number 1 left by (log_N - j - 1) positions (<< operator)
                     *  - set the j-th bit in the reversed_index registry to 1
                     * why (log_N - j - 1)?
                     * it is used to reverse the position of the bit in the binary representation of the index;
                     * therefore, if an index has n bits, the bit at position 0 of the original index should be moved
                     * to position n-1 in the inverted index;
                     * the bit at position 1 should be moved to position n-2, and so on.
                     *  - log_N: is the total number of bits required to represent the index
                     *           (base 2 logarithmic is a well-posed assumption, since N is a power of 2).
                     *  - j: is the current bit position in the original index.
                     *  - log_N - j - 1: is the position of the bit in the reversed index.
                     */
                    reversed_index |= (1 << (log_N - j - 1));
                }

            }

            /**
             * swapping
             * - why if condition?
             *      ~ bit-reversal algorithm is symmetric because for any given index i,
             *        there exists a unique index reversed_index such that reversing the
             *        bits of i gives reversed_index (and vice versa);
             *        simply put, the bit-reversal algorithm is a bijective function.
             *      ~ to avoid swapping the same element twice,
             *        we need to check if the reversed index is greater than i;
             * - example:
             *      ~ N = 8, log_N = 3:
             *          ~ i = 0 (000), reversed_index = 0 (000) - no swap
             *          ~ i = 1 (001), reversed_index = 4 (100) - swap
             *          ~ i = 2 (010), reversed_index = 2 (010) - no swap
             *          ~ i = 3 (011), reversed_index = 6 (110) - swap
             *          ~ i = 4 (100), reversed_index = 1 (001) - no swap
             *          ~ i = 5 (101), reversed_index = 5 (101) - no swap
             *          ~ i = 6 (110), reversed_index = 3 (011) - no swap
             *          ~ i = 7 (111), reversed_index = 7 (111) - no swap
             *        Swapping results:
             *          ~ i = 0 (000), reversed_index = 0 (000)
             *          x i = 4 (100), reversed_index = 1 (001)
             *          ~ i = 2 (010), reversed_index = 2 (010)
             *          x i = 6 (110), reversed_index = 3 (011)
             *          x i = 1 (001), reversed_index = 4 (100)
             *          ~ i = 5 (101), reversed_index = 5 (101)
             *          x i = 3 (011), reversed_index = 6 (110)
             *          ~ i = 7 (111), reversed_index = 7 (111)
             *        As we can see, the elements at indices 0, 2, 4, 5, 6, and 7 are not swapped,
             *        but are in order because the bit-reversal algorithm is symmetric.
             */
            if (reversed_index > i) {
                // swap the element at index i with the element at the reversed index
                // use utility function std::swap to swap the elements:
                // https://en.cppreference.com/w/cpp/algorithm/swap
                std::swap(data[i], data[reversed_index]);
            }
        }
        return data;
    }

    std::vector<std::complex<double>> & parallelBitReversal(std::vector<std::complex<double>> &data) {
        // algorithm without comments and with OpenMP parallelization
        // see sequential version for commented version
        const size_t N = data.size();
        const size_t log_N = std::log2(N);
        /**
         * the pragma omp parallel for directive is used to parallelize the for loop.
         *
         * the parallelization of the bit-reversal algorithm focuses on the outer loop;
         * each iteration of the outer loop (for different values of i) is independent of the others,
         * so that it can be executed in parallel by different threads (thread safety is guaranteed).
         */
    #pragma omp parallel for
        for (size_t i = 0; i < N; ++i) {
            size_t reversed_index = 0;
            for (size_t j = 0; j < log_N; ++j) {
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
}
