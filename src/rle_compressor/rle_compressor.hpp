#ifndef RLE_COMPRESSOR_HPP
#define RLE_COMPRESSOR_HPP
#include <vector>

class RLEcompressor{
public:
    RLEcompressor() = default;
    static const std::vector<std::pair<int, int>> compress(const std::vector<double>& input);
    static const std::vector<double> decompress(const std::vector<std::pair<int, int>>& compressed);
};

#endif //RLE_COMPRESSOR_HPP