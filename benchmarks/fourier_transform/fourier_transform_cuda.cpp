#ifndef HAS_CUDA // if CUDA is not available, exit
#error "CUDA is not available. Please compile with CUDA support."
#endif

#include <benchmark/benchmark.h>
#include <random>
#include <cuda_runtime_api.h>
#include <string>
#include <sstream>

#include "utils.hpp"
#include "transforms/fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp"
#include "utils/timestamp.hpp"

using namespace signal_processing::fft::solver;

constexpr size_t MAX_TOTAL_SIZE = 4194304;

template <size_t N>
void benchmarkByDim() {
    auto shapes = generateValidShapes<N>(MAX_TOTAL_SIZE);
    for (const auto& dims : shapes) {
        // create a benchmark name based on the dimensions
        // e.g. "2D/4x4x4"
        std::ostringstream name;
        name << N << "D/";
        for (const size_t d : dims)
            name << d << "x";
        // remove the last 'x'
        name.seekp(-1, std::ios_base::end);

        // ReSharper disable once CppDFAUnusedValue
        benchmark::RegisterBenchmark(name.str().c_str(), [=](benchmark::State& state) {
            FastFourierTransform<N> fft(dims);
            auto input = generateInput(dims);

            for (auto _ : state) {
                auto copy = input;
                fft.compute(copy, ComputationMode::CUDA);
                cudaDeviceSynchronize();
                benchmark::DoNotOptimize(copy);
            }
        });
    }
}

int main(const int argc, char** argv) {
    benchmarkByDim<1>();
    benchmarkByDim<2>();
    benchmarkByDim<3>();

    std::ostringstream oss;
    // use the format provided by the user
    oss << "--benchmark_out=output/results_cuda_"
        << signal_processing::utils::timestamp::createReadableTimestamp("%Y-%m-%d_%H-%M-%S")
        << ".json";
    const std::string benchmark_out = oss.str();

    const char* args[] = {
        argv[0],  // keep program name
        argc > 1 ? argv[1] : nullptr,
        benchmark_out.c_str(),
        "--benchmark_out_format=json"
    };
    int custom_argc = sizeof(args) / sizeof(char*);

    // Initialize with overridden args
    benchmark::Initialize(&custom_argc, const_cast<char**>(args));
    benchmark::RunSpecifiedBenchmarks();
}
