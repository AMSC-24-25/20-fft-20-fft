#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <string>

#include "cooley-tukey-inverse-fft.hpp"  // Assicurati che il path sia corretto
using namespace fft::algorithms::cooley_tukey;

int main() {
    std::ifstream infile("input.txt");
    std::ofstream outfile("output.txt");

    size_t N;
    infile >> N;
    std::vector<std::complex<double>> data(N);

    for (size_t i = 0; i < N; ++i) {
        double real, imag;
        infile >> real >> imag;
        data[i] = std::complex<double>(real, imag);
    }

    computeCooleyTurkeyInverseFFT(data);

    for (const auto& c : data) {
        outfile << c.real() << " " << c.imag() << "\n";
    }

    return 0;
}
