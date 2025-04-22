#include <iostream>
#include <vector>
#include <cmath>

class HaarWaveletTransform2D{

    public:
    std::vector<std::vector<double>> input;
    std::vector<std::vector<double>> solution;
    std::vector<std::vector<double>> Hn;

    HaarWaveletTransform2D(std::vector<std::vector<double>> inputMatrix): 
    input(inputMatrix),
    solution(inputMatrix),
    Hn(inputMatrix.size(), std::vector<double>(inputMatrix[0].size(), 0))
    {
        constructHn();
    }

    std::vector<std::vector<double>> getInput(){return input;}
    std::vector<std::vector<double>> getSolution(){return solution;}
    std::vector<std::vector<double>> getHnMatrix(){return Hn;}

    void compute(){
        
        double size = 1;
        size /= Hn.size();

        solution = matmul(Hn, solution);
        transpose(Hn);
        solution = matmul(solution, Hn);
        transpose(Hn);

        for(int i = 0; i < Hn.size(); i++){
            for(int j = 0; j < Hn.size(); j++){

                if(std::abs(solution[i][j]) < 1e-10) solution[i][j] = 0;
                else solution[i][j] *= size;
            }
        }
            
    }

    void transpose(std::vector<std::vector<double>>& mat){

        std::vector<std::vector<double>> temp(mat.size(), std::vector<double>(mat[0].size()));;
        int r = mat.size(), c = mat[0].size();
        for(int i = 0; i < r; i++)
            for(int j = 0; j < c; j++)
                temp[i][j] = mat[j][i];

        mat = temp;
    }

    private:
    void constructHn(){

        int steps = std::log2(Hn.size());

        Hn[0][0] = 1;
        Hn[0][1] = 1;
        Hn[1][0] = 1;
        Hn[1][1] = -1;
    
        for(int n = 2; n <= steps; n++)
            HnStep(n);

    }


    void HnStep (int n){

        int size = std::exp2(n-1);
        std::vector<std::vector<double>> temp(size, std::vector<double>(size, 0.0));
        std::vector<std::vector<double>> identity(size, std::vector<double>(size, 0.0));
        std::vector<double> v1 = {1, 1};
        std::vector<double> v2 = {1, -1};
        double coeff = sqrt(exp2(n-1));
            
        for(int i = 0; i < identity.size(); i++)
            identity[i][i] = coeff;
    
        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                temp[i][j] = Hn[i][j];
    
        std::vector<std::vector<double>> U = KronekerProduct(temp, v1);    
        std::vector<std::vector<double>> L = KronekerProduct(identity, v2);
    
        for(int i = 0; i < U.size(); i++)
            for(int j = 0; j < U[0].size(); j++){
    
                Hn[i][j] = U[i][j];
                Hn[U.size() + i][j] = L[i][j];
            }
    }

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