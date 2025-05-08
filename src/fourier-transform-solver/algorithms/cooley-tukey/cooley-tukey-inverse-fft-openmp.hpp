#ifndef COOLEY_TURKEY_INVERSE_FFT_OPENMP_HPP
#define COOLEY_TURKEY_INVERSE_FFT_OPENMP_HPP

#include <complex>
#include <vector>

namespace fft::algorithms::cooley_tukey {
    /**
     * Parallel Cooley-Tukey Inverse Fast Fourier Transform (IFFT) Algorithm using OpenMP (1D).
     *
     * It handles only the
     * <a href="https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm#The_radix-2_DIT_case">radix-2 case</a>.
     *
     * The output is stored in the same input vector, which is modified in place.
     *
     * @param input The input vector of complex numbers.
     */
    void computeCooleyTurkeyInverseFFTOpenMP(std::vector<std::complex<double>>& input);
}

#endif //COOLEY_TURKEY_INVERSE_FFT_OPENMP_HPP
