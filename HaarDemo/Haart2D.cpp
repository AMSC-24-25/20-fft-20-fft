#include <iostream>
#include <vector>
#include <cmath>

class HaarWaveletTransform2D{

    public:
    std::vector<std::vector<double>> input;
    std::vector<std::vector<double>> solution;

    HaarWaveletTransform2D(std::vector<std::vector<double>> inputMatrix): input(inputMatrix), solution(inputMatrix){}

    std::vector<std::vector<double>> getInput(){return input;}
    std::vector<std::vector<double>> getSolution(){return solution;}

    void compute(){

        transformRows();
        transpose();
        transformRows();
        transpose();
    }

    void transpose(){

        std::vector<std::vector<double>> temp(input.size(), std::vector<double>(input[0].size()));;
        int r = input.size(), c = input[0].size();
        for(int i = 0; i < r; i++)
            for(int j = 0; j < c; j++)
                temp[i][j] = solution[j][i];

        solution = temp;
    }

    void transformRows(){

        int rows = input.size();
        int size;

        for(int i = 0; i < rows; i++){

            size = input[0].size();

            while(size > 1){

                solution[i] = haartStep(size, solution[i]);
                size = size / 2;
            }
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