#include "rle_compressor.hpp"
#include <vector>
#include <iostream>

/* Function that performs run-length encoding (RLE) on a vector.
 * Compresses the input by storing pairs of (count, value) for repeated consecutive values.
 * @input: input vector of doubles to compress
 */
const std::vector<std::pair<int, int>> RLEcompressor::compress(const std::vector<double>& input) {
    std::vector<std::pair<int, int>> compressed; // (#value_repetitions, value)
    int count = 1;

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == input[i+1]) {
            count++;
            if(input[i] == 0){
            }
        } else {
            compressed.emplace_back(count, input[i]); //vector.emplace_back(element) --> adds a constructed-in-place element to the end of the vector
            count = 1;
        }
    }

    return compressed;
}

/* Function that decompresses a run-length encoded (RLE) vector.
 * Expands pairs of (count, value) back into the original sequence.
 * @compressed: input vector of (count, value) pairs
 */
const std::vector<double> RLEcompressor::decompress(const std::vector<std::pair<int, int>>& compressed) {
    std::vector<double> decompressed;

    for (const auto& [repetitions, value] : compressed) {
        decompressed.insert(decompressed.end(), repetitions, value); //vector.insert(pos, #repetitions, value) --> insert in position pos the #repetitions of value
        //decompressed.push_back(value); //vector.push_back(value) --> insert element value at the end of the vector
    }

    return decompressed;
}