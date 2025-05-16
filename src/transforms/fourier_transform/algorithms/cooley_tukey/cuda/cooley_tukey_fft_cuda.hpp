#ifndef CUDA_FFT_HPP
#define CUDA_FFT_HPP

#include <complex>
#include <vector>

namespace signal_processing::fft::algorithms::cooley_tukey {
    /**
     * Cooley-Tukey Fast Fourier Transform (FFT) Algorithm using CUDA (1D).
     *
     * It handles only the
     * <a href="https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm#The_radix-2_DIT_case">radix-2 case</a>.
     *
     * The output is stored in the same input vector, which is modified in place.
     *
     * @param input The input vector of complex numbers.
     */
    void computeFFTCuda(std::vector<std::complex<double>>& input);

    /**
     * Cooley-Tukey Inverse Fast Fourier Transform (IFFT) Algorithm using CUDA (1D).
     *
     * It handles only the
     * <a href="https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm#The_radix-2_DIT_case">radix-2 case</a>.
     *
     * The output is stored in the same input vector, which is modified in place.
     *
     * @param input The input vector of complex numbers.
     */
    void computeInverseFFTCuda(std::vector<std::complex<double>>& input);
}

#endif //CUDA_FFT_HPP
