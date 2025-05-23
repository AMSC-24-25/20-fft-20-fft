/**
 * @file fourier_transform_solver_performance.cpp
 * @brief A simple demo to measure the performance of the FFT solver in sequential and parallel modes.
 *
 * @details The demo generates a signal in time or space domain, computes the FFT and IFFT of the signal,
 *          and measures the time taken for each operation.
 *          It is not intended to be a comprehensive performance test, but rather a simple demonstration of the
 *          performance of the FFT solver.
 */

#include <iostream>
#include <cmath>
#include <omp.h>

#include "signal_processing/signal_processing.hpp"

/**
 * This demo measures the performance of the FFT solver in sequential and parallel modes.
 */
void sequential_vs_parallel_fft(const std::vector<std::complex<double>>& signal){
    const int signal_length = signal.size();
    // prepare the vectors for the FFT
    std::vector<std::complex<double>> sequential_fft(signal_length);
    std::vector<std::complex<double>> parallel_fft(signal_length);
    signal_processing::fft::solver::FastFourierTransform<1> solver(std::array{static_cast<size_t>(signal_length)});
    // you can use the solver in two modes:
    // 1. in-place computation (default): the input vector is modified in place and
    //                                    the result is stored in the same vector
    // 2. out-of-place computation: the input vector is not modified and the result is stored in a new vector
    const auto start_time_seq = std::chrono::high_resolution_clock::now();
    solver.compute(signal, sequential_fft, signal_processing::fft::solver::ComputationMode::SEQUENTIAL);
    const auto end_time_seq = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for sequential FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()
    );
    const auto start_time_par = std::chrono::high_resolution_clock::now();
    solver.compute(signal, parallel_fft, signal_processing::fft::solver::ComputationMode::OPENMP);
    const auto end_time_par = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for parallel (OpenMP, CPU) FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    printf(
        "With %d threads (CPU), the speedup is: %ld\n",
        omp_get_max_threads(),
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count() /
           std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    // uncomment the following lines to save the result to a file
    // const CsvSignalSaver csv_signal_saver;
    // csv_signal_saver.saveToFile(sequential_fft, "examples/output/sequential_fft_signal");
    // csv_signal_saver.saveToFile(parallel_fft, "examples/output/parallel_fft_signal");
}

/**
 * This demo measures the performance of the Inverse FFT solver in sequential and parallel modes.
 */
void sequential_vs_parallel_inverse_fft(const std::vector<std::complex<double>>& signal){
    const int signal_length = signal.size();
    // prepare the vectors for the Inverse FFT
    std::vector<std::complex<double>> sequential_ifft(signal_length);
    std::vector<std::complex<double>> parallel_ifft(signal_length);
    // prepare the solver
    signal_processing::fft::solver::InverseFastFourierTransform<1> inverse_solver(
        std::array{static_cast<size_t>(signal_length)}
    );
    // sequential Inverse FFT
    const auto start_time_seq = std::chrono::high_resolution_clock::now();
    inverse_solver.compute(
        signal, sequential_ifft, signal_processing::fft::solver::ComputationMode::SEQUENTIAL
    );
    const auto end_time_seq = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for sequential Inverse FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()
    );
    const auto start_time_par = std::chrono::high_resolution_clock::now();
    inverse_solver.compute(
        signal, parallel_ifft, signal_processing::fft::solver::ComputationMode::OPENMP
    );
    const auto end_time_par = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for parallel (OpenMP, CPU) Inverse FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    printf(
        "With %d threads (CPU), the speedup is: %ld\n",
        omp_get_max_threads(),
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count() /
           std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    // uncomment the following lines to save the result to a file
    // const CsvSignalSaver csv_signal_saver;
    // csv_signal_saver.saveToFile(sequential_ifft, "examples/output/sequential_ifft_signal");
    // csv_signal_saver.saveToFile(parallel_ifft, "examples/output/parallel_ifft_signal");
}


int main() {
    // ============================================= Configuration Loading =============================================
    // load the configuration from the sample file
    // print number of threads used by OpenMP
    const std::string file_path = "examples/resources/performance_fft_config.json";
    // load the configuration from the file
    const auto loader = new signal_processing::config::JSONConfigurationLoader();
    loader->loadConfigurationFromFile(file_path);
    const auto json_loaded = new signal_processing::config::JsonFieldHandler(loader->getConfigurationData());
    // get the simulation parameters
    const int signal_length = json_loaded->getSignalLength();
    const double *frequency = new double(json_loaded->getHzFrequency());
    const double *phase = new double(json_loaded->getPhase());
    const double *noise = new double(json_loaded->getNoise());
    const std::string *signal_domain = new std::string(json_loaded->getSignalDomain());
    // get the seed if it exists, otherwise set it to nullopt
    const std::optional<int> seed = json_loaded->hasSeed() ? std::optional(json_loaded->getSeed()) : std::nullopt;
    // prepare the signal saver and use the unique pointer to manage the memory
    const auto csv_signal_saver = std::make_unique<signal_processing::handlers::signal_saver::CsvSignalSaver>();
    // free unused memory
    delete loader;
    delete json_loaded;


    // ================================================ Generate Signal ================================================
    // generate the signal
    std::vector<std::complex<double>> signal(signal_length);
    if (*signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        signal_processing::handlers::signal_generator::TimeDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, *frequency, *phase, *noise);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        signal_processing::handlers::signal_generator::SpaceDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, *frequency, *phase, *noise);
    }
    // uncomment the following line to save the generated signal to a file
    // csv_signal_saver->saveToFile(signal, "examples/output/input_signal");


    // ========================================== Sequential vs. Parallel FFT ==========================================
    printf("\n\nSequential vs. Parallel FFT\n");
    sequential_vs_parallel_fft(signal);


    // ====================================== Sequential vs. Parallel Inverse FFT ======================================
    printf("\n\nSequential vs. Parallel Inverse FFT\n");
    // pass the fft result as input
    std::vector<std::complex<double>> fft_res(signal_length);
    signal_processing::fft::solver::FastFourierTransform<1> tmp_solver(std::array{static_cast<size_t>(signal_length)});
    sequential_vs_parallel_inverse_fft(fft_res);

    return 0;
}
