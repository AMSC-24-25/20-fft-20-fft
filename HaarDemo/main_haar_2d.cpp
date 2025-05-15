#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "Haart2D.cpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> power_dist(2, 5); // N = 4 ... 32
    std::uniform_real_distribution<> val_dist(-10.0, 10.0);

    int N = std::pow(2, power_dist(gen));
    std::vector<std::vector<double>> matrix(N, std::vector<double>(N));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = val_dist(gen);

    std::cout << "Input matrix (" << N << "x" << N << "):\n";
    for (const auto& row : matrix) {
        for (double val : row) std::cout << val << "\t";
        std::cout << "\n";
    }

    HaarWaveletTransform2D transform(matrix);
    transform.compute();
    auto result = transform.getSolution();

    std::cout << "\nHaar Transform matrix:\n";
    for (const auto& row : result) {
        for (double val : row) std::cout << val << "\t";
        std::cout << "\n";
    }

    return 0;
}
