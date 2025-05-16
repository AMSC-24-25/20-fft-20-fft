#ifndef IMAGE_COMPRESSION_HAAR_WAVELET_HPP
#define IMAGE_COMPRESSION_HAAR_WAVELET_HPP

namespace signal_processing::compression::hwt {
    class ImgWLComp {
    public:
        std::vector<std::vector<double>> input;         //input matrix (original image)
        std::vector<std::vector<double>> compressed;    //compressed image matrix
        std::vector<std::vector<double>> decompressed;  //decompressed (reconstructed) image matrix
        std::vector<std::vector<double>> HaarMatrix = {{1, 1}, {1, -1}}; // Basic 2x2 Haar matrix

        //constructor: initialize input, compressed, and decompressed matrices
        explicit ImgWLComp(std::vector<std::vector<double>> inputMatrix):
            input(inputMatrix),
            compressed(inputMatrix),
            decompressed(inputMatrix)
        {}

        //getter for compressed matrix
        std::vector<std::vector<double>> getCompressed() {
            return compressed;
        }

        //getter for decompressed matrix
        std::vector<std::vector<double>> getDecompressed() {
            return decompressed;
        }

        //function to compress the image
        void compress();

        //function to reconstruct the image from compressed data
        //note: still under development (marked as "Non funziona" = not working)
        void reconstruct();

        //helper function to transpose a matrix
        void transpose(std::vector<std::vector<double>>& mat);

        //function to reconstruct all rows of the matrix
        void reconstructRows();

        //perform one inverse step of the Haar transform for a row
        std::vector<double> invertStep(int size, std::vector<double> data);

        //apply the Haar transform to all rows
        void transformRows();

        //perform one Haar transform step on a row
        std::vector<double> haartStep(int step, std::vector<double> data);

        //utility to print a vector
        void showV(std::vector<double> vec);

        //utility to print a matrix
        void showM(std::vector<std::vector<double>> mat);
    };
}

#endif //IMAGE_COMPRESSION_HAAR_WAVELET_HPP
