#include <iostream>
#include <vector>
#include <cmath>

class ImgWLComp{

    public:
    std::vector<std::vector<double>> input;
    std::vector<std::vector<double>> compressed;
    std::vector<std::vector<double>> decompressed;
    std::vector<std::vector<double>> HaarMatrix = {{1, 1}, {1, -1}};

    ImgWLComp(std::vector<std::vector<double>> inputMatrix): input(inputMatrix), compressed(inputMatrix), decompressed(inputMatrix){}

    std::vector<std::vector<double>> getCompressed(){return compressed;}

    std::vector<std::vector<double>> getDecompressed(){return decompressed;}

    void compress(){

        transformRows();
        transpose(compressed);
        transformRows();
        transpose(compressed);

        for(int i = 0; i < input.size(); i++)
            for(int j = 0; j < input[0].size(); j++)
                if(std::abs(compressed[i][j]) <= 5) compressed[i][j] = 0;/**/
    }

    //Non funziona
    void reconstruct(){

        decompressed = compressed;

        transpose(decompressed);
        reconstructRows();
        transpose(decompressed);
        reconstructRows();/**/
        }

    void transpose(std::vector<std::vector<double>>& mat){

    std::vector<std::vector<double>> temp(mat.size(), std::vector<double>(mat[0].size()));;
    int r = mat.size(), c = mat[0].size();
    for(int i = 0; i < r; i++)
        for(int j = 0; j < c; j++)
            temp[i][j] = mat[j][i];

    mat = temp;
    }

    void reconstructRows(){

        int rows = input.size();
        int size;
        int step = 1;

        for(int i = 0; i < rows; i++){

            size = 2;
            while(size <= rows){

                decompressed[i] = invertStep(size, decompressed[i]);
                size = size * 2;
            }
        }
    }

    std::vector<double> invertStep(int size, std::vector<double> data){

        std::vector<double> temp = data;
        int half = size/2;

        for(int i = 0; i < half; i++){

            temp[2*i] = data[half + i] + data[i];
            temp[2*i + 1] = 2*data[i] - temp[2*i];
        }

        return temp;
    }

    void transformRows(){

        int rows = input.size();
        int size;

        for(int i = 0; i < rows; i++){

            size = input[0].size();

           while(size > 1){

                compressed[i] = haartStep(size, compressed[i]);
                size = size / 2;
            }/**/
        }    
    }

    std::vector<double> haartStep(int step, std::vector<double> data){

        int size = data.size();
        std::vector<double> tmp(size);
        
        for(int i = 0; i < size; i++) 
            tmp[i] = data[i];

        for (int i=0 ; i < step/2 ; i++){
            tmp[i] = (data[2*i + 1] + data[2*i] ) / 2;
            tmp[step/2 + i] = (data[(2*i)] - data[(2*i + 1)]) / 2;
        }

        return tmp;
    }

    void showV(std::vector<double> vec){

        std::cout << "vector= [ ";

        for(int i = 0; i < vec.size(); i++)
            std::cout << vec[i] << " ";

        std::cout << "]\n";
    }

    void showM(std::vector<std::vector<double>> mat){

    std::cout << "Matrix: " << std::endl << std::endl << "\t";
    for(int i = 0; i < mat.size(); i++){
        for(int j = 0; j < mat[0].size(); j++){
            std::cout << mat[i][j] << "\t";
        }
        std::cout << std::endl << "\t";
    }
    std::cout << std::endl << std::endl;
    }
};