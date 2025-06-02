#include <vector>
#include <iostream>

#include "rle_compressor.hpp"

namespace sp::utils::rle
{
    const std::vector<std::pair<int, int>> RLECompressor::compress(const std::vector<double>& input) {
        // (#value_repetitions, value)
        std::vector<std::pair<int, int>> compressed;
        int count = 1;

        for (size_t i = 0; i < input.size(); ++i) {
            if (input[i] == input[i+1]) {
                count++;
                if(input[i] == 0){
                }
            } else {
                //vector.emplace_back(element) --> adds a constructed-in-place element to the end of the vector
                compressed.emplace_back(count, input[i]);
                count = 1;
            }
        }

        return compressed;
    }

    const std::vector<double> RLECompressor::decompress(const std::vector<std::pair<int, int>>& compressed) {
        std::vector<double> decompressed;

        for (auto it = compressed.begin(); it != compressed.end(); ++it) {
            decompressed.insert(
                decompressed.end(),
                it->first,  // repetitions
                it->second  // value
            );
            //decompressed.push_back(value); //vector.push_back(value) --> insert element value at the end of the vector
        }

        return decompressed;
    }
}
