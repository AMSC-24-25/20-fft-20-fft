#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "signal_processing/transforms/haar_wavelet_transform/haar_wavelet_1d.hpp"

int main() {
    // Random: scegli un'esponente da 3 a 6 → lunghezze 8, 16, 32, 64
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> power_dist(3, 6); // 2^3 = 8 ... 2^6 = 64
    std::uniform_real_distribution<> val_dist(-10.0, 10.0);

    int N = std::pow(2, power_dist(gen)); // lunghezza casuale potenza di 2
    std::vector<double> input(N);
    for (int i = 0; i < N; ++i)
        input[i] = val_dist(gen);

    std::cout << "Input vector (" << N << " elements): ";
    for (double v : input) std::cout << v << " ";
    std::cout << "\n\n";

    signal_processing::hwt::solver::HaarWaveletTransform1D wavelet(input);
    wavelet.compute();
    auto output = wavelet.getSolution();

    std::cout << "Haar Transform output:\n";
    for (double v : output) std::cout << v << " ";
    std::cout << "\n";

    return 0;
}
