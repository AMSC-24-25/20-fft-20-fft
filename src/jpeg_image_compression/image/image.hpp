#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

class CompressedImage;

class Image {
    public:
        std::vector<std::vector<double>> img_matrix;	        //input matrix (original image)

        //constructor
        Image() = default;                                      //default constructor
        Image(std::vector<std::vector<double>> inputMatrix);	//constructor: initialize image from a matrix
        Image(const char* image_path);                          //constructor: initialize image from file .png

        const void save_as_png(const std::string path);
        CompressedImage compress();

    private:
        void jpeg_compression(const int r, const int c, const int submatrixSize, std::vector<std::vector<double>>& compressed, std::vector<std::vector<double>>& Q);
        const std::vector<std::vector<double>> load_from_png(const char* image_path);
};

#endif //IMAGE_HPP