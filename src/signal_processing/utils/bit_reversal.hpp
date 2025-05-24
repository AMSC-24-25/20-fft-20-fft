#ifndef BIT_REVERSAL_HPP
#define BIT_REVERSAL_HPP
#include <complex>
#include <vector>

/**
 * Bit-Reversal Algorithm.
 *
 * This module provides functions to perform bit-reversal on a vector of complex numbers.
 */
namespace sp::utils::bit_rev
{
    /**
    * Perform a sequential bit-reversal on the input data vector.
    *
    * The operation is done in-place and the input data vector is modified.
    * The returned vector is done for convenience.
    * @param data The input data vector.
    * @return The input data vector after bit-reversal. It is modified in-place,
    *         so the return value is for convenience.
    */
    std::vector<std::complex<double>> & sequentialBitReversal(std::vector<std::complex<double>> &data);

    /**
     * Perform a parallel (OpenMP) bit-reversal on the input data vector.
     *
     * The operation is done in-place and the input data vector is modified.
     * The returned vector is done for convenience.
     * @param data The input data vector.
     * @return The input data vector after bit-reversal.
     *         It is modified in-place, so the return value is for convenience.
     */
    std::vector<std::complex<double>> & parallelBitReversal(std::vector<std::complex<double>> &data);
}

#endif //BIT_REVERSAL_HPP
