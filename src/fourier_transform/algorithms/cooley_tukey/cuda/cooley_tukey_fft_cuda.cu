#include <vector>
#include <complex>
#include <cuComplex.h>
#include <cuda_runtime.h>
#include <cmath>
#include <stdexcept>
#include "cooley_tukey_fft_cuda.hpp"

namespace fft::algorithms::cooley_tukey
{
    __device__ cuDoubleComplex W(int N, int k) {
        double angle = -2.0 * M_PI * k / N;
        return make_cuDoubleComplex(cos(angle), sin(angle));
    }

    __global__ void conjugate_and_scale(cuDoubleComplex* data, int N, bool scale) {
        int tid = threadIdx.x + blockIdx.x * blockDim.x;
        if (tid < N) {
            data[tid] = cuConj(data[tid]);
            if (scale) {
                data[tid].x /= N;
                data[tid].y /= N;
            }
        }
    }

    __global__ void fft_kernel(cuDoubleComplex* data, int N, int logN) {
        int tid = threadIdx.x + blockIdx.x * blockDim.x;

        for (int s = 1; s <= logN; ++s) {
            int m = 1 << s;
            int m2 = m >> 1;

            if (tid < N / 2) {
                int j = tid % m2;
                int k = tid - j + (tid / m2) * m2;

                cuDoubleComplex t = cuCmul(W(m, j), data[k + m2]);
                cuDoubleComplex u = data[k];
                data[k]       = cuCadd(u, t);
                data[k + m2]  = cuCsub(u, t);
            }

            __syncthreads();
        }
    }

    void computeFFTCuda(std::vector<std::complex<double>>& input) {
        const int N = input.size();

        int logN = static_cast<int>(std::log2(N));

        cuDoubleComplex* d_data;
        cudaMalloc(&d_data, N * sizeof(cuDoubleComplex));

        std::vector<cuDoubleComplex> tmp(N);
        for (int i = 0; i < N; ++i)
            tmp[i] = make_cuDoubleComplex(input[i].real(), input[i].imag());

        cudaMemcpy(d_data, tmp.data(), N * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);

        int threads = 256;
        int blocks = (N + threads - 1) / threads;
        fft_kernel<<<blocks, threads>>>(d_data, N, logN);
        cudaDeviceSynchronize();

        cudaMemcpy(tmp.data(), d_data, N * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);
        for (int i = 0; i < N; ++i)
            input[i] = std::complex<double>(cuCreal(tmp[i]), cuCimag(tmp[i]));

        cudaFree(d_data);
    }

    void computeInverseFFTCuda(std::vector<std::complex<double>>& input) {
        const int N = input.size();

        int logN = static_cast<int>(std::log2(N));

        cuDoubleComplex* d_data;
        cudaMalloc(&d_data, N * sizeof(cuDoubleComplex));

        std::vector<cuDoubleComplex> tmp(N);
        for (int i = 0; i < N; ++i)
            tmp[i] = make_cuDoubleComplex(input[i].real(), input[i].imag());

        cudaMemcpy(d_data, tmp.data(), N * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);

        int threads = 256;
        int blocks = (N + threads - 1) / threads;

        // Conjugate input
        conjugate_and_scale<<<blocks, threads>>>(d_data, N, false);

        // Forward FFT
        fft_kernel<<<blocks, threads>>>(d_data, N, logN);
        cudaDeviceSynchronize();

        // Conjugate again and scale by 1/N
        conjugate_and_scale<<<blocks, threads>>>(d_data, N, true);
        cudaDeviceSynchronize();

        cudaMemcpy(tmp.data(), d_data, N * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);
        for (int i = 0; i < N; ++i)
            input[i] = std::complex<double>(cuCreal(tmp[i]), cuCimag(tmp[i]));

        cudaFree(d_data);
    }
}
