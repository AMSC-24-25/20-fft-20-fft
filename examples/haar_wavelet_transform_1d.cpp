#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "signal_processing/signal_processing.hpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution power_dist(3, 6); // 2^3 = 8 ... 2^6 = 64
    std::uniform_real_distribution val_dist(-10.0, 10.0);

    int N = std::pow(2, power_dist(gen));
    std::vector<double> input(N);
    for (int i = 0; i < N; ++i)
        input[i] = val_dist(gen);

    std::cout << "Input vector (" << N << " elements): ";
    for (double v : input) std::cout << v << " ";
    std::cout << "\n\n";

    sp::hwt::solver::HaarWaveletTransform1D wavelet(input);
    wavelet.compute();
    auto output = wavelet.getSolution();

    std::cout << "Haar Transform output:\n";
    for (double v : output) std::cout << v << " ";
    std::cout << "\n";

    return 0;
}
