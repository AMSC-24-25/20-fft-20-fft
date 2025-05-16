#include "zigzag_scan.hpp"
#include <vector>
#include <iostream>

namespace signal_processing::utils::zigzag_scan
{
    const std::vector<double> ZigZagScan::scan(const std::vector<std::vector<double>>& matrix) {
        std::vector<double> result;
        if(matrix.empty()){
            return result;
        }

        int rows = matrix.size();
        int cols = matrix[0].size();
        int r = 0;
        int c = 0;
        bool goingDown = false;

        for(int i = 0; i<rows*cols; ++i){
            result.push_back(matrix[r][c]);

            if(goingDown){
                if(r == rows-1){
                    // we have reached the bottom edge of the matrix
                    goingDown = false;
                    c++;
                }
                else if(c == 0){
                    // we have reached the left edge of the matrix
                    goingDown = false;
                    r++;
                }
                else{
                    //we go down following the current diagonal
                    r++;
                    c--;
                }
            }
            else{ //goingUp
                if(r == 0){
                    //we have reached the top edge of the matrix
                    goingDown = true;
                    c++;
                }
                else if(c == cols-1){
                    // we have reached the right edge of the matrix
                    goingDown = true;
                    r++;
                }
                else{
                    //we go up following the current diagonal
                    r--;
                    c++;
                }
            }
        }

        return result;
    }

    const std::vector<std::vector<double>> ZigZagScan::inverse_scan(
        const std::vector<double>& scanned,
        const int rows,
        const int cols
    ) {
        if (scanned.size() != rows * cols) {
            throw std::runtime_error(
                "Error: the size of the zigzag scanned vector does not correspond to the matrix sizes"
            );
        }

        std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
        int r = 0;
        int c = 0;
        int idx = 0;
        int goingDown = false;

        for (int i = 0; i<rows*cols; ++i){
            matrix[r][c] = scanned[idx];
            idx++;

            if(goingDown) {
                if(r == rows-1) {
                    // we have reached the bottom edge of the matrix
                    goingDown = false;
                    c++;
                }
                else if(c == 0) {
                    // we have reached the left edge of the matrix
                    goingDown = false;
                    r++;
                } else {
                    //we go down following the current diagonal
                    r++;
                    c--;
                }
            } else { //goingUp
                if(r == 0) {
                    //we have reached the top edge of the matrix
                    goingDown = true;
                    c++;
                } else if(c == cols-1) {
                    // we have reached the right edge of the matrix
                    goingDown = true;
                    r++;
                } else {
                    //we go up following the current diagonal
                    r--;
                    c++;
                }
            }
        }

        return matrix;
    }
}
