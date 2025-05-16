#ifndef COMPRESSED_IMAGE_HPP
#define COMPRESSED_IMAGE_HPP

#include <string>
#include <vector>

namespace signal_processing::compression::jpeg_image_compression
{
    class Image; //forward declaration

    class CompressedImage {
    public:
        std::vector<std::vector<double>> compressed;    //compressed image matrix

        // default constructor
        CompressedImage() = default;
        /**
         * Constructor that initializes the CompressedImage object from a given matrix by copying it.
         *
         * @param inputMatrix: 2D vector containing pixel values.
         */
        explicit CompressedImage(std::vector<std::vector<double>> inputMatrix);

        /**
         * Constructor that loads the compressed image from a file .bin
         * @param compressed_image_path:  path to binary file;
         * @param option: 1 (raw binary), 2 (compressed binary).
         */
        CompressedImage(const std::string& compressed_image_path, int option);

        /**
         * Function that saves compressed as a binary file.
         *
         * @param path: binary file path.
         */
        const void save_as_binary(const std::string& path);

        /**
         * Function that saves compressed as a compressed binary file (with zigzag + RLE).
         *
         * @param path: compressed binary file path.
         */
        const void save_as_compressed_binary(const std::string& path);

        /**
         * Function that implements the jpeg decompression algorithm on compressed image matrix.
         *
         * @return: decompressed image.
         */
        Image decompress();

    private:
        /**
         * Function that decompresses a single 8x8 block using JPEG (dequantization + inverse DCT).
         *
         * The output is directly copy into the corresponding position of decompressed.
         *
         * @param r: position in image_data of the first row of the current submatrix.
         * @param c: position in image_data of the first column of the current submatrix.
         * @param submatrixSize: block size (is a squared block).
         * @param decompressed: decompressed image matrix.
         * @param Q: quantization matrix.
         */
        void jpeg_decompression(
            int r,
            int c,
            int submatrixSize,
            std::vector<std::vector<double>>& decompressed,
            std::vector<std::vector<double>>& Q
        );

        /**
         * Function that saves a vector using RLE to a binary file.
         * The vector corresponds to zigzag scan of a submatrix.
         *
         * @param file: binary file stream.
         * @param zzVector: zigzag-scanned vector.
         */
        const void save_compressed_submatrix(std::ofstream& file, const std::vector<double>& zzVector);

        /**
         * Function that loads a compressed image from a binary file into a matrix (vector of vector).
         *
         * The binary file contains the image data in a raw format.
         *
         * @param path: path to the binary file.
         * @return: the matrix containing the image.
         */
        const std::vector<std::vector<double>> load_from_binary(const std::string& path);

        /**
         * Function that loads a compressed matrix from a compressed binary file (zigzag + rle).
         *
         * @param path: the path to the compressed binary file.
         * @return: the matrix containing the image.
         */
        const std::vector<std::vector<double>> load_from_compressed_binary(const std::string& path);

        /**
         * Function that loads a submatrix from the compressed binary file stream.
         *
         * It returns a vector of size vectorSize, containing the zigzag scan of the submatrix.
         *
         * @param file: reference to the input binary file stream.
         * @param vectorSize: number of elements to read in the file.
         * @return: the vector containing the zigzag scan of the submatrix.
         */
        const std::vector<double> load_compressed_submatrix(std::ifstream& file, int vectorSize);
    };
}

#endif //COMPRESSED_IMAGE_HPP
