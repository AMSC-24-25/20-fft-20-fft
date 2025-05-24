#ifndef IMAGE_COMPRESSION_HAAR_WAVELET_HPP
#define IMAGE_COMPRESSION_HAAR_WAVELET_HPP

#include <vector>

namespace sp::hwt {
    class ImgWLComp {
    public:
        //constructor: initialize input, compressed, and decompressed matrices
        explicit ImgWLComp(){}

        // Compresses the input image matrix using Haar wavelet transform and thresholding
        std::vector<std::vector<double>> compress(std::vector<std::vector<double>> input);

        // Reconstructs the original image matrix from its compressed (transformed) version
        std::vector<std::vector<double>> reconstruct(std::vector<std::vector<double>> input);

        // Saves the transformed image matrix to a binary file with basic RLE compression
        void save_as_binary(std::vector<std::vector<double>> img_matrix, const std::string& path);

        // Loads a matrix from a binary file and reconstructs the image data with scaling
        std::vector<std::vector<double>> load_img_from_binary(std::string compressed_image_path);

        // Transposes a given matrix (rows become columns and vice versa)
        void transpose(std::vector<std::vector<double>>& mat);

        // Applies inverse Haar wavelet transform to each row of the matrix
        void reconstructRows(std::vector<std::vector<double>>& input);

        // Performs one step of inverse Haar wavelet transform on a single row
        std::vector<double> invertStep(int size, std::vector<double>& data);

        // Applies forward Haar wavelet transform to each row of the matrix
        void transformRows(std::vector<std::vector<double>>& input);

        // Performs one step of forward Haar wavelet transform on a single row
        std::vector<double> haartStep(int step, std::vector<double>& data);
    };
}

#endif //IMAGE_COMPRESSION_HAAR_WAVELET_HPP
