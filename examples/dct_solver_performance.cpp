/**
 * @file dct_solver_performance.cpp
 * @brief A simple demo to measure the performance of the DCT solver in sequential and parallel modes.
 *
 * @details The demo generates a signal in time or space domain, computes the DCT and IDCT of the signal,
 *          and measures the time taken for each operation.
 *          It is not intended to be a comprehensive performance test, but rather a simple demonstration of the
 *          performance of the DCT solver.
 */

#include <chrono>
#include <iostream>
#include <cmath>
#include <omp.h>
#include "matplot/matplot.h"

#include "signal_processing/signal_processing.hpp"

/**
 * This demo measures the performance of the DCT solver in sequential and parallel modes.
 */
void sequential_vs_parallel_dct(const std::vector<double>& signal, std::vector<double>& sequential_dct, std::vector<double>& parallel_dct){
    // Prepare the solver
    sp::dct::solver::DiscreteCosineTransform dct_solver= sp::dct::solver::DiscreteCosineTransform();
    // you can use the solver in two modes:
    // 1. in-place computation (default): the input vector is modified in place and
    //                                    the result is stored in the same vector
    // 2. out-of-place computation: the input vector is not modified and the result is stored in a new vector

    // ================================================ Sequential DCT ================================================
    const auto start_time_seq = std::chrono::high_resolution_clock::now();
    dct_solver.compute(signal, sequential_dct, sp::dct::solver::ComputationMode::SEQUENTIAL);
    const auto end_time_seq = std::chrono::high_resolution_clock::now();

    printf(
        "Time taken for sequential DCT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()
    );

    // ================================================= Parallel DCT =================================================
    const auto start_time_par = std::chrono::high_resolution_clock::now();
    dct_solver.compute(signal, parallel_dct, sp::dct::solver::ComputationMode::OPENMP);
    const auto end_time_par = std::chrono::high_resolution_clock::now();

    printf(
        "Time taken for parallel (OpenMP, CPU) DCT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    printf(
        "With %d threads (CPU), the speedup is: %ld\n",
        omp_get_max_threads(),
        (std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()) /
           (std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count())
    );

    // uncomment the following lines to save the result to a file
    // const CsvSignalSaver csv_signal_saver;
    // csv_signal_saver.saveToFile(sequential_dct, "examples/output/sequential_dct_signal");
    // csv_signal_saver.saveToFile(parallel_dct, "examples/output/parallel_dct_signal");
}

/**
 * This demo measures the performance of the Inverse DCT solver in sequential and parallel modes.
 */
void sequential_vs_parallel_inverse_dct(const std::vector<double>& sequential_dct, const std::vector<double>& parallel_dct, std::vector<double>& sequential_idct, std::vector<double>& parallel_idct){
    // Prepare the solver
    sp::dct::solver::InverseDiscreteCosineTransform idct_solver= sp::dct::solver::InverseDiscreteCosineTransform();

    // ============================================ Sequential Inverse DCT ============================================
    const auto start_time_seq = std::chrono::high_resolution_clock::now();
    idct_solver.compute(
        sequential_dct, sequential_idct, sp::dct::solver::ComputationMode::SEQUENTIAL
    );
    const auto end_time_seq = std::chrono::high_resolution_clock::now();

    printf(
        "Time taken for sequential Inverse DCT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()
    );

    // ============================================= Parallel Inverse DCT =============================================
    const auto start_time_par = std::chrono::high_resolution_clock::now();
    idct_solver.compute(
        parallel_dct, parallel_idct, sp::dct::solver::ComputationMode::OPENMP
    );
    const auto end_time_par = std::chrono::high_resolution_clock::now();

    printf(
        "Time taken for parallel (OpenMP, CPU) Inverse DCT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    printf(
        "With %d threads (CPU), the speedup is: %ld\n\n",
        omp_get_max_threads(),
        (std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()) /
            (std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count())
    );

    // uncomment the following lines to save the result to a file
    // const CsvSignalSaver csv_signal_saver;
    // csv_signal_saver.saveToFile(sequential_idct, "examples/output/sequential_idct_signal");
    // csv_signal_saver.saveToFile(parallel_idct, "examples/output/parallel_idct_signal");
}

// Function that scales DCT coefficients to frequency
std::vector<double> scale_dct_frequencies(int num_coefficients, double sampling_rate, double signal_duration) {
    std::vector<double> scaled_freq(num_coefficients);
    for (int i = 0; i < num_coefficients; ++i) {
        scaled_freq[i] = static_cast<double>(i) * (sampling_rate / (2.0 * (sampling_rate * signal_duration)));
    }
    return scaled_freq;
}

int main() {
    // Set parameters for signal generation
    double signal_duration = 1.0; // signal duration in seconds
    double sampling_rate = 500.0; // sampling rate in Hz
    // Calculate the number of samples
    int signal_length = static_cast<int>(signal_duration * sampling_rate);

    //Generate signal(t) = sin(2*pi*10*t) + sin(2*pi*20*t) + sin(2*pi*30*t)+sin(2*pi*50*t)+2
    std::vector<double> signal(signal_length);
    for (int i = 0; i < signal_length; ++i) {
        double t = static_cast<double>(i) / sampling_rate; // calculate the time at each sample
        signal[i] = std::sin(2.0 * M_PI * 10.0 * t) + std::sin(2.0 * M_PI * 20.0 * t) + std::sin(2.0 * M_PI * 30.0 * t) + std::sin(2.0 * M_PI * 50.0 * t) + 2.0;
    }

    std::vector<double> sequential_dct(signal_length);
    std::vector<double> parallel_dct(signal_length);
    std::vector<double> sequential_idct(signal_length);
    std::vector<double> parallel_idct(signal_length);

/*
    // ============================================= Configuration Loading =============================================
    // load the configuration from the sample file
    // print number of threads used by OpenMP
    const std::string file_path = "examples/resources/performance_fft_config.json";
    // load the configuration from the file
    const auto loader = new sp::config::JSONConfigurationLoader();
    loader->loadConfigurationFromFile(file_path);
    const auto json_loaded = new sp::config::JsonFieldHandler(loader->getConfigurationData());
    // get the simulation parameters
    const int signal_length = json_loaded->getSignalLength();
    const double *frequency = new double(json_loaded->getHzFrequency());
    const double *phase = new double(json_loaded->getPhase());
    const double *noise = new double(json_loaded->getNoise());
    const std::string signal_domain = json_loaded->getSignalDomain();
    // get the seed if it exists, otherwise set it to nullopt
    const std::optional<int> seed = json_loaded->hasSeed() ? std::optional(json_loaded->getSeed()) : std::nullopt;
    // prepare the signal saver and use the unique pointer to manage the memory
    const auto csv_signal_saver = sp::saver::CsvSignalSaver();
    // free unused memory
    delete loader;
    delete json_loaded;


    // ================================================ Generate Signal ================================================
    // generate the signal
    std::vector<double> signal(signal_length);
    if (signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        sp::signal_gen::TimeDomainSignalGenerator domain_signal_generator(
            json_loaded->hasSeed() ? json_loaded->getSeed() : 0
        );
        signal = domain_signal_generator.generateReal1DSignal(signal_length, *frequency, *phase, *noise);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        sp::signal_gen::SpaceDomainSignalGenerator domain_signal_generator(
            json_loaded->hasSeed() ? json_loaded->getSeed() : 0
        );
        signal = domain_signal_generator.generateReal1DSignal(signal_length, *frequency, *phase, *noise);
    }
*/

    // uncomment the following line to save the generated signal to a file
    // csv_signal_saver->saveToFile(signal, "examples/output/input_signal");


    // ========================================== Sequential vs. Parallel DCT ==========================================
    printf("\n\nSequential vs. Parallel DCT\n");
    sequential_vs_parallel_dct(signal, sequential_dct, parallel_dct);


    // ====================================== Sequential vs. Parallel Inverse DCT ======================================
    printf("\n\nSequential vs. Parallel Inverse DCT\n");
    sequential_vs_parallel_inverse_dct(sequential_dct, parallel_dct, sequential_idct, parallel_idct);



    // =================================================== Plotting ===================================================
    // Linear space for time axis
    std::vector<double> t = matplot::linspace(0, signal_duration, signal_length);
    // Calculate scaled frequency axis for DCT
    std::vector<double> scaled_freq = scale_dct_frequencies(signal_length, sampling_rate, signal_duration);

    // create the plot for magnitude comparison
    auto magnitude_figure = matplot::figure();
    // title of the window
    magnitude_figure->name("Magnitude 1D DCT Signal Comparison");
    // title of the plot
    magnitude_figure->title("Magnitude Comparison");
    // size of the window (width, height)
    magnitude_figure->size(1300, 700);
    // position of the window (x, y)
    magnitude_figure->x_position(0);
    magnitude_figure->y_position(0);
    // plot
    magnitude_figure->add_subplot(3,1,0);
    matplot::plot(t, signal);
    matplot::title("Original Signal");
    matplot::ylabel("Amplitude");
    matplot::xlabel("Time [s]");
    magnitude_figure->add_subplot(3,1,1);
    matplot::plot(scaled_freq, sequential_dct);
    matplot::title("Sequential DCT");
    matplot::ylabel("Magnitude");
    matplot::xlabel("Frequency [Hz]");
    magnitude_figure->add_subplot(3,1,2);
    matplot::plot(scaled_freq, parallel_dct);
    matplot::title("Parallel DCT");
    matplot::ylabel("Magnitude");
    matplot::xlabel("Frequency [Hz]");
    // show the plot and block the execution
    magnitude_figure->show();

    // create the plot for original signal comparison
    auto comparison_idct_figure = matplot::figure();
    // title of the window
    comparison_idct_figure->name("Plot 1D FFT Signal Comparison");
    // title of the plot
    comparison_idct_figure->title("Original Signal vs. Inverse FFT Signal");
    // size of the window (width, height)
    comparison_idct_figure->size(1300, 700);
    // position of the window (x, y)
    comparison_idct_figure->x_position(0);
    comparison_idct_figure->y_position(0);
    // plot
    comparison_idct_figure->add_subplot(3,1,0);
    matplot::plot(t, signal);
    matplot::title("Original Signal");
    matplot::xlabel("Time [s]");
    matplot::ylabel("Amplitude");
    comparison_idct_figure->add_subplot(3,1,1);
    matplot::plot(t, sequential_idct);
    matplot::title("Sequential IDCT");
    matplot::xlabel("Time [s]");
    matplot::ylabel("Amplitude");
    comparison_idct_figure->add_subplot(3,1,2);
    matplot::plot(t, parallel_idct);
    matplot::title("Parallel IDCT");
    matplot::xlabel("Time [s]");
    matplot::ylabel("Amplitude");
    // show the plot and block the execution
    comparison_idct_figure->show();

    return 0;
}