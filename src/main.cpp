#include <iostream>
#include "config-loader/json-configuration-loader.hpp"
#include "fourier-transform-solver/parallel-1d-fast-ft.hpp"
#include "fourier-transform-solver/parallel-1d-inverse-fast-ft.hpp"
#include "fourier-transform-solver/sequential-1d-fast-ft.hpp"
#include "fourier-transform-solver/sequential-1d-inverse-fast-ft.hpp"
#include "matplot/matplot.h"
#include "signal-generator/space-domain-signal-generator.hpp"
#include "signal-generator/time-domain-signal-generator.hpp"
#include "signal-saver/csv-signal-saver.hpp"

/**
 * Environment variable name for the file path for the configuration file.
 */
#define ENV_FILE_PATH "CONFIG_FILE_PATH_FFT"

int main() {
    // ============================================= Configuration Loading =============================================
    // get the file path from environment variable
    if (getenv(ENV_FILE_PATH) == nullptr) {
        std::cerr << "Warning: Environment variable " << ENV_FILE_PATH << " is not set. "
                                                                          "Using default configuration file path.\n";
        // if the environment variable is not set, use the default configuration file path
        setenv(ENV_FILE_PATH, "resources/sample-config.json", 0);
    }
    const std::string filePath = getenv(ENV_FILE_PATH);

    // load the configuration from the file
    const auto loader = new JSONConfigurationLoader();
    loader->loadConfigurationFromFile(filePath);
    const auto json_loaded = new JsonFieldHandler(loader->getConfigurationData());
    // free unused memory
    delete loader;
    // get the simulation parameters
    const int signal_length = json_loaded->getSignalLength();
    const double *frequency = new double(json_loaded->getHzFrequency());
    const double *phase = new double(json_loaded->getPhase());
    const double *noise = new double(json_loaded->getNoise());
    const std::string *signal_domain = new std::string(json_loaded->getSignalDomain());
    // get the seed if it exists
    const std::optional<int> seed = json_loaded->hasSeed() ? std::optional(json_loaded->getSeed()) : std::nullopt;
    // prepare the signal saver and use the unique pointer to manage the memory
    const auto csv_signal_saver = std::make_unique<CsvSignalSaver>();
    // free unused memory
    delete json_loaded;


    // ================================================ Generate Signal ================================================
    // generate the signal
    std::vector<std::complex<double>> signal(signal_length);
    if (*signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        TimeDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, *frequency, *phase, *noise);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        SpaceDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generate1DSignal(signal_length, *frequency, *phase, *noise);
    }
    // free unused memory
    delete frequency;
    delete phase;
    delete noise;
    delete signal_domain;
    // and save it to a file
    csv_signal_saver->saveToFile(signal, "output/input_signal");
    // since the transformation is in-place, we need to keep a copy of the original signal
    std::vector<std::complex<double>> original_signal(signal_length);
    std::vector<std::complex<double>> sequential_fft_input(signal_length);
    std::vector<std::complex<double>> parallel_fft_input(signal_length);
    std::vector<std::complex<double>> parallel_inverse_fft_input(signal_length);
    std::vector<std::complex<double>> sequential_inverse_fft_input(signal_length);
    for (size_t i = 0; i < signal_length; ++i) {
        original_signal[i] = signal[i];
        sequential_fft_input[i] = signal[i];
        parallel_fft_input[i] = signal[i];
        parallel_inverse_fft_input[i] = signal[i];
        sequential_inverse_fft_input[i] = signal[i];
    }


    // ================================================ Sequential FFT ================================================
    printf("\n\nFFT\n");
    auto *solver = new Sequential1DFastFT(sequential_fft_input);
    solver->compute();
    const std::vector<std::complex<double>> &solution = solver->getSolution();
    csv_signal_saver->saveToFile(solution, "output/fft_signal");
    // free the memory
    delete solver;


    // ================================================= Parallel FFT =================================================
    printf("\n\nParallel - FFT\n");
    auto *parallel_solver = new Parallel1DFastFT(parallel_fft_input);
    parallel_solver->compute();
    const std::vector<std::complex<double>> &parallel_solution = parallel_solver->getSolution();
    csv_signal_saver->saveToFile(parallel_solution, "output/parallel_fft_signal");
    // free the memory
    delete parallel_solver;

    // ============================================ Sequential Inverse FFT ============================================
    printf("\n\nSequential - Inverse FFT\n");
    auto *tmp_solver_seq = new Sequential1DFastFT(sequential_inverse_fft_input);
    tmp_solver_seq->compute();
    // use the FFT result as input
    auto *inverse_solver_seq = new Sequential1DInverseFastFT(tmp_solver_seq->getSolution());
    inverse_solver_seq->compute();
    const std::vector<std::complex<double>> inverse_solution_seq = inverse_solver_seq->getSolution();
    csv_signal_saver->saveToFile(inverse_solution_seq, "output/seq_inverse_fft_signal");
    // free the memory
    delete inverse_solver_seq;
    delete tmp_solver_seq;


    // ============================================= Parallel Inverse FFT =============================================
    printf("\n\nParallel - Inverse FFT\n");
    auto *tmp_solver = new Sequential1DFastFT(parallel_inverse_fft_input);
    tmp_solver->compute();
    // use the FFT result as input
    auto *inverse_solver = new Parallel1DInverseFastFT(tmp_solver->getSolution());
    inverse_solver->compute();
    const std::vector<std::complex<double>> inverse_solution = inverse_solver->getSolution();
    csv_signal_saver->saveToFile(inverse_solution, "output/parallel_inverse_fft_signal");
    // free the memory
    delete inverse_solver;
    delete tmp_solver;


    // ====================== Plotting ======================
    // prepare the data for plotting
    // original signal comparison:
    std::vector<double> plot_original_signal(signal_length),
                        plot_inverse_signal_parallel(signal_length),
                        plot_inverse_signal_sequential(signal_length);
    // FFT signal comparison:
    // sequential FFT
    std::vector<double> plot_sequential_fft_magnitude(signal_length), plot_sequential_fft_phase(signal_length);
    // parallel FFT
    std::vector<double> plot_parallel_fft_magnitude(signal_length), plot_parallel_fft_phase(signal_length);
    // sequential Inverse FFT:
    std::vector<double> plot_sequential_inverse_fft_magnitude(signal_length),
                        plot_sequential_inverse_fft_phase(signal_length);
    // parallel Inverse FFT:
    std::vector<double> plot_parallel_inverse_fft_magnitude(signal_length),
                        plot_parallel_inverse_fft_phase(signal_length);
    // linear space for x-axis
    std::vector<double> x = matplot::linspace(0, signal_length - 1, signal_length);
    // fill the data
    for (size_t i = 0; i < signal_length; ++i) {
        // comparison of the original signal
        plot_original_signal[i] = original_signal[i].real();
        plot_inverse_signal_parallel[i] = inverse_solution[i].real();
        plot_inverse_signal_sequential[i] = inverse_solution_seq[i].real();
        // magnitude
        plot_sequential_fft_magnitude[i] = std::abs(sequential_fft_input[i]);
        plot_parallel_fft_magnitude[i] = std::abs(parallel_fft_input[i]);
        plot_parallel_inverse_fft_magnitude[i] = std::abs(parallel_inverse_fft_input[i]);
        plot_sequential_inverse_fft_magnitude[i] = std::abs(sequential_inverse_fft_input[i]);
        // phase
        plot_sequential_fft_phase[i] = std::arg(sequential_fft_input[i]);
        plot_parallel_fft_phase[i] = std::arg(parallel_fft_input[i]);
        plot_parallel_inverse_fft_phase[i] = std::arg(parallel_inverse_fft_input[i]);
        plot_sequential_inverse_fft_phase[i] = std::arg(sequential_inverse_fft_input[i]);
    }

    // create the plot for original signal comparison
    auto comparison_figure = matplot::figure();
    // title of the window
    comparison_figure->name("Plot 1D FFT Signal Comparison");
    // title of the plot
    comparison_figure->title("Original Signal vs. Inverse FFT Signal");
    // size of the window (width, height)
    comparison_figure->size(1200, 800);
    // position of the window (x, y)
    comparison_figure->x_position(0);
    comparison_figure->y_position(0);
    // plot
    comparison_figure->add_subplot(3,1,1);
    matplot::plot(x, plot_original_signal);
    matplot::title("Original Signal");
    comparison_figure->add_subplot(3,1,2);
    matplot::plot(x, plot_inverse_signal_sequential);
    matplot::title("Sequential IFFT");
    comparison_figure->add_subplot(3,1,3);
    matplot::plot(x, plot_inverse_signal_parallel);
    matplot::title("Parallel IFFT");
    // show the plot and block the execution
    comparison_figure->show();


    // create the plot for magnitude comparison
    auto magnitude_figure = matplot::figure();
    // title of the window
    magnitude_figure->name("Magnitude 1D FFT Signal Comparison");
    // title of the plot
    magnitude_figure->title("Magnitude Comparison");
    // size of the window (width, height)
    magnitude_figure->size(1300, 800);
    // position of the window (x, y)
    magnitude_figure->x_position(0);
    magnitude_figure->y_position(0);
    // plot
    magnitude_figure->add_subplot(2,2,1);
    matplot::plot(x, plot_sequential_fft_magnitude);
    matplot::title("Sequential FFT");
    magnitude_figure->add_subplot(2,2,2);
    matplot::plot(x, plot_parallel_fft_magnitude);
    matplot::title("Parallel FFT");
    magnitude_figure->add_subplot(2,2,3);
    matplot::plot(x, plot_sequential_inverse_fft_magnitude);
    matplot::title("Inverse Sequential FFT");
    magnitude_figure->add_subplot(2,2,4);
    matplot::plot(x, plot_parallel_inverse_fft_magnitude);
    matplot::title("Inverse Parallel FFT");
    // show the plot and block the execution
    magnitude_figure->show();


    // create the plot for phase comparison
    auto phase_figure = matplot::figure();
    // title of the window
    phase_figure->name("Phase 1D FFT Signal Comparison");
    // title of the plot
    phase_figure->title("Phase Comparison");
    // size of the window (width, height)
    phase_figure->size(1300, 800);
    // position of the window (x, y)
    phase_figure->x_position(0);
    phase_figure->y_position(0);
    // plot
    phase_figure->add_subplot(2,2,1);
    matplot::plot(x, plot_sequential_fft_phase);
    matplot::title("Sequential FFT");
    phase_figure->add_subplot(2,2,2);
    matplot::plot(x, plot_parallel_fft_phase);
    matplot::title("Parallel FFT");
    phase_figure->add_subplot(2,2,3);
    matplot::plot(x, plot_sequential_inverse_fft_phase);
    matplot::title("Inverse Sequential FFT");
    phase_figure->add_subplot(2,2,4);
    matplot::plot(x, plot_parallel_inverse_fft_phase);
    matplot::title("Inverse Parallel FFT");
    // show the plot and block the execution
    phase_figure->show();

    return 0;
}
