#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

namespace signal_processing::compression::jpeg_image_compression
{
    class CompressedImage;

    class Image {
    public:
        /**
         * Input matrix (original image) is a 2D vector of doubles.
         */
        std::vector<std::vector<double>> img_matrix;

        // default constructor
        Image() = default;

        /**
         * Constructor that initializes the Image object from a given matrix by copying it.
         *
         * @param inputMatrix: 2D vector containing pixel values.
         */
        explicit Image(std::vector<std::vector<double>> inputMatrix);

        /**
         * Constructor that loads a PNG file into the variable image of Image object.
         *
         * @param image_path: path to the PNG image file.
         */
        explicit Image(const char* image_path);

        /**
         * Function that saves the image matrix of the current object as a PNG file using stb_image_write.
         *
         * @path: path for the PNG file.
         */
        const void save_as_png(std::string path);

        /**
         * Function that implements the JPEG compression algorithm on the full image matrix.
         *
         * @return: compressed image.
         */
        CompressedImage compress();

    private:
        /**
         * Function that compresses a single 8x8 block using JPEG (DCT + quantization).
         *
         * The output is directly saved into the corresponding position of compressed.
         *
         * @param r: position in image of the first row of the current submatrix.
         * @param c: position in image of the first column of the current submatrix.
         * @param submatrixSize: block size (is a squared block).
         * @param compressed: compressed image matrix.
         * @param Q: quantization matrix.
         */
        void jpeg_compression(
            int r,
            int c,
            int submatrixSize,
            std::vector<std::vector<double>>& compressed,
            std::vector<std::vector<double>>& Q
        );

        /**
         * Function that loads an image from a PNG file into a matrix using stb_image.
         *
         * @param image_path: path to the PNG file.
         * @return 2D vector containing pixel values.
         */
        const std::vector<std::vector<double>> load_from_png(const char* image_path);
    };
}

#endif //IMAGE_HPP