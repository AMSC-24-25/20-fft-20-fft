#ifndef HAAR_WAVELET_1D_HPP
#define HAAR_WAVELET_1D_HPP

namespace sp::hwt::solver {
    class HaarWaveletTransform1D {
    public:
        std::vector<double> input;     //input vector
        std::vector<double> solution;  //vector to store the solution (transformed data)

        //constructor: initializes input and solution vectors
        explicit HaarWaveletTransform1D(
            std::vector<double> inputVector
        ):input(inputVector), solution(inputVector){}

        //getter for solution vector
        std::vector<double> getSolution() {
            return solution;
        }

        //getter for input vector
        std::vector<double> getInput() {
            return input;
        }

        //main function to compute the Haar wavelet transform
        void compute();

        //perform one step of the Haar transform
        std::vector<double> haartStep(int step, std::vector<double> data);
    };
}

#endif //HAAR_WAVELET_1D_HPP
