#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

//TODO: risolvi problema incoerenza tra image/compressed/decompressed
//esempio: se creo oggetto ImageJPEG a partire da image, poi comprimo + decomprimo, ed infine carico una nuova immagine come image genero incoerenza

class ImageJPEG {
    public:
        std::vector<std::vector<double>> image;         //input matrix (original image)
        std::vector<std::vector<double>> compressed;    //compressed image matrix
        std::vector<std::vector<double>> decompressed;  //decompressed (reconstructed) image matrix

        //constructor
        ImageJPEG() = default;                                                  //default constructor
        ImageJPEG(std::vector<std::vector<double>> inputMatrix);                //constructor: initialize image from a matrix
        ImageJPEG(const char* image_path);                                      //constructor: initialize image from file .png
        ImageJPEG(const std::string compressed_image_path, const int option);   //constructor: initialize compressed from file .bin

        //void save_as_binary();
        const void save_image_as_png(const std::string path);
        const void save_decompressed_as_png(const std::string path);
        const void save_compressed_as_binary(const std::string& path);
        const void save_compressed_as_compressed_binary(const std::string& path);

        //void load_compressed_from_binary(const std::string& path);
        //void load_compressed_from_compressed_binary(const std::string& path);

        void compress();
        void decompress();

    private:
        void jpeg_compression(const int r, const int c, const int submatrixSize, std::vector<std::vector<double>>& Q);
        void jpeg_decompression(const int r, const int c, const int submatrixSize, std::vector<std::vector<double>>& Q);

        const void save_as_png(const std::vector<std::vector<double>>& img_matrix, const std::string path);
        const void save_as_binary(std::vector<std::vector<double>>& img_matrix, const std::string& path);
        const void save_as_compressed_binary(std::vector<std::vector<double>>& img_matrix, const std::string& path);
        const void save_compressed_submatrix(std::ofstream& file, const std::vector<double>& zzVector);

        const std::vector<std::vector<double>> load_from_png(const char* image_path);
        const std::vector<std::vector<double>> load_from_binary(const std::string path);
        const std::vector<std::vector<double>> load_from_compressed_binary(const std::string& path);
        const std::vector<double> load_compressed_submatrix(std::ifstream& file, const int vectorSize);
};

#endif
