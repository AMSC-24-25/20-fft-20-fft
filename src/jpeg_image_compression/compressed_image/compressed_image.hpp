#ifndef COMPRESSED_IMAGE_HPP
#define COMPRESSED_IMAGE_HPP

#include <string>
#include <vector>

class Image; //forward declaration

class CompressedImage {
    public:
        std::vector<std::vector<double>> compressed;    //compressed image matrix

        // default constructor
        CompressedImage() = default;
        CompressedImage(std::vector<std::vector<double>> inputMatrix);
        CompressedImage(const std::string compressed_image_path, const int option);

        const void save_as_binary(const std::string& path);
        const void save_as_compressed_binary(const std::string& path);

        Image decompress();

    private:
        void jpeg_decompression(
            const int r,
            const int c,
            const int submatrixSize,
            std::vector<std::vector<double>>& decompressed,
            std::vector<std::vector<double>>& Q
        );

        const void save_compressed_submatrix(std::ofstream& file, const std::vector<double>& zzVector);

        const std::vector<std::vector<double>> load_from_binary(const std::string& path);
        const std::vector<std::vector<double>> load_from_compressed_binary(const std::string& path);
        const std::vector<double> load_compressed_submatrix(std::ifstream& file, const int vectorSize);
};

#endif //COMPRESSED_IMAGE_HPP
