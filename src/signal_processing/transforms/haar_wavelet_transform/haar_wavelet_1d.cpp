#include <iostream>
#include <cmath>
#include <vector>

namespace signal_processing::hwt::solver
{
    class HaarWaveletTransform1D {

    public:
        std::vector<double> input;     //input vector
        std::vector<double> solution;  //vector to store the solution (transformed data)

        //constructor: initializes input and solution vectors
        HaarWaveletTransform1D(std::vector<double> inputVector):input(inputVector), solution(inputVector){}

        //getter for solution vector
        std::vector<double> getSolution() {return solution;}

        //getter for input vector
        std::vector<double> getInput() {return input;}

        //main function to compute the Haar wavelet transform
        void compute(){

            int size = input.size();

            //check if size is valid (must be even and at least 2)
            if (size < 2 || size % 2 != 0) {
                std::cerr << "Size must be even and >= 2." << std::endl;
                return;
            }

            //perform successive Haar transform steps
            while (size > 1){
                solution = haartStep(size, solution);
                size = size / 2;
            }
        }

        //perform one step of the Haar transform
        std::vector<double> haartStep(int step, std::vector<double> data){

            int size = data.size();
            std::vector<double> tmp(size);

            //copy data to temporary vector
            for(int i = 0; i < size; i++)
                tmp[i] = data[i];

            //apply the Haar transformation for the given step
            for (int i=0 ; i < step/2 ; i++){
                tmp[i] = (data[2*i + 1] + data[2*i] ) / sqrt(2);         //compute average
                tmp[step/2 + i] = (data[(2*i)] - data[(2*i + 1)]) / sqrt(2); //compute difference
            }

            return tmp; //return the transformed vector
        }
    };
}
