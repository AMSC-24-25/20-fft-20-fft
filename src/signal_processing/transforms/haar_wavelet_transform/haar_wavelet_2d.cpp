#include <iostream>
#include <vector>
#include <cmath>

namespace signal_processing::hwt::solver
{
    class HaarWaveletTransform2D {

    public:
        std::vector<std::vector<double>> input;     //input matrix
        std::vector<std::vector<double>> solution;  //matrix to store the transformed result
        std::vector<std::vector<double>> Hn;        //haar matrix

        //constructor: initializes input, solution, and Hn matrix
        HaarWaveletTransform2D(std::vector<std::vector<double>> inputMatrix):
        input(inputMatrix),
        solution(inputMatrix),
        Hn(inputMatrix.size(), std::vector<double>(inputMatrix[0].size(), 0))
        {
            constructHn(); //build the initial Haar matrix
        }

        //getter for input matrix
        std::vector<std::vector<double>> getInput(){return input;}

        //getter for solution matrix
        std::vector<std::vector<double>> getSolution(){return solution;}

        //getter for Haar matrix
        std::vector<std::vector<double>> getHnMatrix(){return Hn;}

        //main function to compute the 2D Haar transform
        void compute(){

            double size = 1;
            size /= Hn.size();

            solution = matmul(Hn, solution); //apply Haar transform on rows
            transpose(Hn);                   //transpose Haar matrix
            solution = matmul(solution, Hn); //apply Haar transform on columns
            transpose(Hn);                   //restore original Haar matrix

            //normalize and remove very small values
            for(int i = 0; i < Hn.size(); i++){
                for(int j = 0; j < Hn.size(); j++){

                    if(std::abs(solution[i][j]) < 1e-10) solution[i][j] = 0;
                    else solution[i][j] *= size;
                }
            }
        }

        //function to transpose a matrix
        void transpose(std::vector<std::vector<double>>& mat){

            std::vector<std::vector<double>> temp(mat.size(), std::vector<double>(mat[0].size()));
            int r = mat.size(), c = mat[0].size();
            for(int i = 0; i < r; i++)
                for(int j = 0; j < c; j++)
                    temp[i][j] = mat[j][i];

            mat = temp;
        }

        private:
        //function to build the Haar matrix Hn
        void constructHn(){

            int steps = std::log2(Hn.size());

            //initialize H2 matrix
            Hn[0][0] = 1;
            Hn[0][1] = 1;
            Hn[1][0] = 1;
            Hn[1][1] = -1;

            //build higher order Haar matrices
            for(int n = 2; n <= steps; n++)
                HnStep(n);
        }

        //one step in building a higher-order Haar matrix
        void HnStep (int n){

            int size = std::exp2(n-1);
            std::vector<std::vector<double>> temp(size, std::vector<double>(size, 0.0));
            std::vector<std::vector<double>> identity(size, std::vector<double>(size, 0.0));
            std::vector<double> v1 = {1, 1};   //vector for upper block
            std::vector<double> v2 = {1, -1};  //vector for lower block
            double coeff = sqrt(exp2(n-1));

            //create identity matrix scaled by coeff
            for(int i = 0; i < identity.size(); i++)
                identity[i][i] = coeff;

            //copy current Hn into temp
            for(int i = 0; i < size; i++)
                for(int j = 0; j < size; j++)
                    temp[i][j] = Hn[i][j];

            //create upper and lower parts using Kronecker product
            std::vector<std::vector<double>> U = KronekerProduct(temp, v1);
            std::vector<std::vector<double>> L = KronekerProduct(identity, v2);

            //merge U and L into new Hn
            for(int i = 0; i < U.size(); i++)
                for(int j = 0; j < U[0].size(); j++){

                    Hn[i][j] = U[i][j];
                    Hn[U.size() + i][j] = L[i][j];
                }
        }

        //kronecker product between a matrix and a vector
        std::vector<std::vector<double>> KronekerProduct(std::vector<std::vector<double>>& A, std::vector<double> v){

            int half = A.size();
            int vsize = v.size();
            std::vector<std::vector<double>> result(half, std::vector<double>(half*v.size()));

            for(int i = 0; i < half; i++)
                for(int j = 0; j < half; j++)
                    for(int k = 0; k < vsize; k++)
                        result[i][j*vsize + k] = A[i][j] * v[k];

            return result;
        }

        //matrix multiplication between two matrices
        std::vector<std::vector<double>> matmul(std::vector<std::vector<double>> A, std::vector<std::vector<double>> B){

            int rowsA = A.size();
            int colsA = A[0].size();
            int colsB = B[0].size();

            std::vector<std::vector<double>> temp(rowsA, std::vector<double>(colsB, 0.0));

            for (int i = 0; i < rowsA; i++)
                for (int j = 0; j < colsB; j++)
                    for (int k = 0; k < colsA; k++)
                         temp[i][j] += A[i][k] * B[k][j];

            return temp;
        }
    };
}
