#include <iostream>
#include <benchmark/benchmark.h>
#include <string>
#include <sstream>

#include "utils.hpp"
#include "transforms/fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp"
#include "utils/timestamp.hpp"

using namespace signal_processing::fft::solver;

constexpr size_t MAX_TOTAL_SIZE = 8388608;

/**
 * Combinatorial benchmark for the Fast Fourier Transform (FFT) in N dimensions.
 *
 * This function generates all valid shapes for the FFT solver
 * and registers a benchmark for each shape.
 *
 * Since it is combinatorial, it will generate all possible shapes
 * for the given maximum total size. This means that it will
 * generate shapes unbalanced in terms of dimensions, e.g.: <2, 4194304>
 * @tparam N Number of dimensions for the FFT solver.
 * @param mode The computation mode to be used (SEQUENTIAL, OPENMP).
 * @param min Optional minimum value for the dimensions.
 */
template <size_t N>
void combinatorialBenchmark(ComputationMode mode, const std::optional<int> min = std::nullopt) {
    auto shapes = generateValidShapes<N>(MAX_TOTAL_SIZE, min);
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
                fft.compute(copy, mode);
                benchmark::DoNotOptimize(copy);
            }
        });
    }
}

int main(const int argc, char** argv) {
    if (
        getArgValue(argc, argv, "h").has_value() ||
        getArgValue(argc, argv, "help", true).has_value()
    ) {
        std::cout << "Usage: ./program -dim=<1|2|3> -type=<combinatorial|balanced> -mode=<sequential|openmp>\n";
        return 0;
    }

    const auto dim_opt = getArgValue(argc, argv, "dim");
    const auto type_opt = getArgValue(argc, argv, "type");
    const auto mode_opt = getArgValue(argc, argv, "mode");

    if (!dim_opt || !type_opt || !mode_opt) {
        std::cerr << "Usage: ./program -dim=<1|2|3> -type=<combinatorial|balanced> -mode=<sequential|openmp>\n";
        return 1;
    }

    const size_t dim = std::stoi(*dim_opt);
    const std::string& type = *type_opt;
    const std::string& rawMode = *mode_opt;

    if (dim < 1 || dim > 3) {
        std::cerr << "Invalid dimension. Must be 1, 2, or 3.\n";
        return 1;
    }

    if (type != "combinatorial" && type != "balanced") {
        std::cerr << "Invalid type. Must be 'combinatorial' or 'balanced'.\n";
        return 1;
    }

    if (rawMode != "sequential" && rawMode != "openmp") {
        std::cerr << "Invalid mode. Must be 'sequential', 'openmp'.\n";
        return 1;
    }

    std::ostringstream oss;
    // use the format provided by the user
    oss << "--benchmark_out=" << dim << "D_results_" << rawMode << "_"
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

    const ComputationMode mode = (rawMode == "sequential") ? ComputationMode::SEQUENTIAL : ComputationMode::OPENMP;

    if (type == "balanced") {
        if (dim == 1) {
            combinatorialBenchmark<1>(mode, 256);
        } else if (dim == 2) {
            combinatorialBenchmark<2>(mode, 256);
        } else {
            combinatorialBenchmark<3>(mode, 256);
        }
    } else {
        if (dim == 1) {
            combinatorialBenchmark<1>(mode);
        } else if (dim == 2) {
            combinatorialBenchmark<2>(mode);
        } else {
            combinatorialBenchmark<3>(mode);
        }
    }

    // Initialize with overridden args
    benchmark::Initialize(&custom_argc, const_cast<char**>(args));
    benchmark::RunSpecifiedBenchmarks();
}
