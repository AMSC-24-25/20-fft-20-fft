#ifndef CUDA_FFT_HPP
#define CUDA_FFT_HPP

#include <complex>
#include <vector>

void computeCooleyTurkeyFFTCuda(std::vector<std::complex<double>>& input);
void computeCooleyTurkeyInverseFFTCuda(std::vector<std::complex<double>>& input);

#endif //CUDA_FFT_HPP
