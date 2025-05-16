#ifndef RLE_COMPRESSOR_HPP
#define RLE_COMPRESSOR_HPP
#include <vector>

/**
 * Run-Length Encoding (RLE) Compressor.
 *
 * This module provides functions to perform run-length encoding (RLE) compression
 * and decompression on a vector of doubles.
 */
namespace signal_processing::utils::rle_compressor
{
    class RLECompressor{
    public:
        RLECompressor() = default;

        /**
         * Function that performs run-length encoding (RLE) on a vector.
         * Compresses the input by storing pairs of (count, value) for repeated consecutive values.
         *
         * See also: <a href="https://en.wikipedia.org/wiki/Run-length_encoding">Run-length encoding</a>
         *
         * @param input: input vector of doubles to compress.
         * @return compressed: vector of pairs (count, value) representing the compressed data.
         */
        static const std::vector<std::pair<int, int>> compress(const std::vector<double>& input);

        /**
         * Function that decompresses a run-length encoded (RLE) vector.
         * Expands pairs of (count, value) back into the original sequence.
         *
         * @param compressed: input vector of (count, value) pairs.
         * @return decompressed: vector of doubles representing the decompressed data.
         */
        static const std::vector<double> decompress(const std::vector<std::pair<int, int>>& compressed);
    };
}

#endif //RLE_COMPRESSOR_HPP