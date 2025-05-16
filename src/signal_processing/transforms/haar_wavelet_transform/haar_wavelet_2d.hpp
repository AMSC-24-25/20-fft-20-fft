#ifndef HAAR_WAVELET_2D_HPP
#define HAAR_WAVELET_2D_HPP

namespace signal_processing::hwt::solver {
    class HaarWaveletTransform2D {
    public:
        std::vector<std::vector<double>> input;     //input matrix
        std::vector<std::vector<double>> solution;  //matrix to store the transformed result
        std::vector<std::vector<double>> Hn;        //haar matrix

        //constructor: initializes input, solution, and Hn matrix
        explicit HaarWaveletTransform2D(std::vector<std::vector<double>> inputMatrix):
            input(inputMatrix),
            solution(inputMatrix),
            Hn(inputMatrix.size(), std::vector<double>(inputMatrix[0].size(), 0))
        {
            constructHn(); //build the initial Haar matrix
        }

        //getter for input matrix
        std::vector<std::vector<double>> getInput() {
            return input;
        }

        //getter for solution matrix
        std::vector<std::vector<double>> getSolution() {
            return solution;
        }

        //getter for Haar matrix
        std::vector<std::vector<double>> getHnMatrix() {
            return Hn;
        }

        //main function to compute the 2D Haar transform
        void compute();

        //function to transpose a matrix
        void transpose(std::vector<std::vector<double>>& mat);

        private:
            //function to build the Haar matrix Hn
            void constructHn();

            //one step in building a higher-order Haar matrix
            void HnStep (int n);

            //kronecker product between a matrix and a vector
            std::vector<std::vector<double>> KronekerProduct(
                std::vector<std::vector<double>>& A,
                std::vector<double> v
            );

            //matrix multiplication between two matrices
            std::vector<std::vector<double>> matmul(
                std::vector<std::vector<double>> A,
                std::vector<std::vector<double>> B
            );
    };
}

#endif //HAAR_WAVELET_2D_HPP
