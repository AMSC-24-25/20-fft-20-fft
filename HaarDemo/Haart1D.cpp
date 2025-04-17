#include <iostream>
#include <cmath>
#include <vector>

//using namespace std;

class HaarWaveletTransform1D{

    public:
    std::vector<double> input;
    std::vector<double> solution;

    HaarWaveletTransform1D(std::vector<double> inputVector):input(inputVector), solution(inputVector){}

    std::vector<double> getSolution() {return solution;}
    std::vector<double> getInput() {return input;}

    void compute(){

        int size = input.size();

        if (size < 2 || size % 2 != 0) {
            std::cerr << "Size must be even and >= 2." << std::endl;
            return;
        }

        while (size > 1){
			solution = haartStep(size, solution);
			size = size / 2;
		}
    }

    std::vector<double> haartStep(int step, std::vector<double> data){

        int size = data.size();
        std::vector<double> tmp(size);
        
        for(int i = 0; i < size; i++) 
            tmp[i] = data[i];

        for (int i=0 ; i < step/2 ; i++){
            tmp[i] = (data[2*i + 1] + data[2*i] ) / sqrt(2);
            tmp[step/2 + i] = (data[(2*i)] - data[(2*i + 1)]) / sqrt(2);
        }

        return tmp;
    }
};