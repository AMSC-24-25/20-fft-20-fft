/**
 * @file fourier_transform_solver.cpp
 * @brief This file demonstrates the use of the FFT and IFFT algorithms
 * to compute the FFT and IFFT of a signal.
 *
 * It uses the FFT algorithm (Cooley-Tukey) to compute the FFT of a signal.
 */

#include <iostream>
#include <cmath>

#include "matplot/matplot.h"

#include "config-loader/json-configuration-loader.hpp"
#include "fourier-transform-solver/fast-fourier-transform/fast-fourier-transform.hpp"
#include "fourier-transform-solver/inverse-fast-fourier-transform/inverse-fast-fourier-transform.hpp"
#include "signal-generator/space-domain-signal-generator.hpp"
#include "signal-generator/time-domain-signal-generator.hpp"
#include "signal-saver/csv-signal-saver.hpp"
#include "utils/timestamp.hpp"

/**
 * This demo illustrates the difference between sequential and parallel FFTs.
 *
 * It uses the FFT algorithm (Cooley-Tukey) to compute the FFT of a signal.
 *
 * It computes the FFT in both sequential and parallel modes and measures the time taken for each computation.
 *
 * The FFT library allows you to modify the input vector in place or perform an out-of-place computation.
 * Also, you don't need to create a solver for each mode
 * because you can select a mode when calling the compute function.
 * @param signal The input signal to be transformed.
 */
void sequential_vs_parallel_fft(const std::vector<std::complex<double>>& signal) {
    const int signal_length = signal.size();
    // prepare the vectors for the FFT
    std::vector<std::complex<double>> sequential_fft(signal_length);
    std::vector<std::complex<double>> parallel_fft(signal_length);
    fft::solver::FastFourierTransform<1> solver(std::array{static_cast<size_t>(signal_length)});
    // you can use the solver in two modes:
    // 1. in-place computation (default): the input vector is modified in place and
    //                                    the result is stored in the same vector
    // 2. out-of-place computation: the input vector is not modified and the result is stored in a new vector
    const auto start_time_seq = std::chrono::high_resolution_clock::now();
    solver.compute(signal, sequential_fft, fft::solver::ComputationMode::SEQUENTIAL);
    const auto end_time_seq = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for sequential FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()
    );
    const auto start_time_par = std::chrono::high_resolution_clock::now();
    solver.compute(signal, parallel_fft, fft::solver::ComputationMode::OPENMP);
    const auto end_time_par = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for parallel FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );
    // save the result to a file
    const CsvSignalSaver csv_signal_saver;
    csv_signal_saver.saveToFile(sequential_fft, "examples/output/sequential_fft_signal");
    csv_signal_saver.saveToFile(parallel_fft, "examples/output/parallel_fft_signal");
}

/**
 * This demo illustrates the difference between sequential and parallel Inverse FFTs.
 *
 * It uses the Inverse FFT algorithm (Cooley-Tukey) to compute the Inverse FFT of a signal.
 *
 * It computes the Inverse FFT in both sequential and parallel modes and measures the time taken for each computation.
 *
 * The Inverse FFT library allows you to modify the input vector in place or perform an out-of-place computation.
 * Also, you don't need to create a solver for each mode
 * because you can select a mode when calling the compute function.
 * @param signal The input signal to be transformed.
 */
void sequential_vs_parallel_inverse_fft(const std::vector<std::complex<double>>& signal) {
    const int signal_length = signal.size();
    // prepare the vectors for the Inverse FFT
    std::vector<std::complex<double>> sequential_ifft(signal_length);
    std::vector<std::complex<double>> parallel_ifft(signal_length);
    // prepare the solver
    fft::solver::InverseFastFourierTransform<1> inverse_solver(std::array{static_cast<size_t>(signal_length)});
    // sequential Inverse FFT
    const auto start_time_seq = std::chrono::high_resolution_clock::now();
    inverse_solver.compute(signal, sequential_ifft, fft::solver::ComputationMode::SEQUENTIAL);
    const auto end_time_seq = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for sequential Inverse FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_seq - start_time_seq).count()
    );
    const auto start_time_par = std::chrono::high_resolution_clock::now();
    inverse_solver.compute(signal, parallel_ifft, fft::solver::ComputationMode::OPENMP);
    const auto end_time_par = std::chrono::high_resolution_clock::now();
    printf(
        "Time taken for parallel Inverse FFT: %ld ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time_par - start_time_par).count()
    );

    // save the result to a file
    const CsvSignalSaver csv_signal_saver;
    csv_signal_saver.saveToFile(sequential_ifft, "examples/output/sequential_ifft_signal");
    csv_signal_saver.saveToFile(parallel_ifft, "examples/output/parallel_ifft_signal");
}

/**
 * This demo plots the original signal, the FFT and the Inverse FFT.
 *
 * It uses the Matplot++ library to plot the signals.
 * @param signal The input signal to be transformed.
 * @param signal_domain The domain of the signal (time or space).
 * @param frequency The frequency of the signal.
 * @param phase The phase of the signal.
 * @param noise The noise of the signal.
 */
void plot_signal_fft_and_ifft(
    const std::vector<std::complex<double>>& signal,
    const std::string &signal_domain,
    const double frequency, const double phase, const double noise
) {
    const int signal_length = signal.size();
    // prepare the data for plotting
    std::vector<double> plot_magnitude(signal_length),
        plot_magnitude_fft(signal_length),
        plot_magnitude_ifft(signal_length);
    std::vector<double> plot_phase(signal_length),
        plot_phase_fft(signal_length),
        plot_phase_ifft(signal_length);
    // copy the original signal into the fft_signal
    std::vector<std::complex<double>> fft_signal = signal;
    std::vector<std::complex<double>> inverse_fft_signal(signal_length);
    // prepare the solver
    fft::solver::FastFourierTransform<1> solver(std::array{static_cast<size_t>(signal_length)});
    fft::solver::InverseFastFourierTransform<1> inverse_solver(std::array{static_cast<size_t>(signal_length)});
    // compute the FFT
    // note: the solver is used in place, so the input vector is modified;
    //       the result of the FFT is then used to compute the inverse FFT;
    //       this demonstrates the solver's versatility
    solver.compute(fft_signal, fft::solver::ComputationMode::OPENMP);
    inverse_solver.compute(fft_signal, inverse_fft_signal, fft::solver::ComputationMode::OPENMP);

    // plot the comparison
    const std::vector<double> x = matplot::linspace(0, signal_length - 1, signal_length);
    // convert to real values
    for (int i = 0; i < signal_length; ++i) {
        plot_magnitude[i] = std::abs(signal[i]);
        plot_magnitude_fft[i] = std::abs(fft_signal[i]);
        plot_magnitude_ifft[i] = std::abs(inverse_fft_signal[i]);
        plot_phase[i] = std::arg(signal[i]);
        plot_phase_fft[i] = std::arg(fft_signal[i]);
        plot_phase_ifft[i] = std::arg(inverse_fft_signal[i]);
    }

    const auto comparison_figure = matplot::figure();
    // title of the window
    comparison_figure->name("Plot 1D FFT Signal Comparison");
    // title of the plot
    std::ostringstream title;
    title << "Original 1D Signal vs. IFFT and FFT (" << signal_domain << " domain, L = "
          << signal_length << ", f = "
          << std::fixed << std::setprecision(2) << frequency << " Hz, phase = "
          << std::fixed << std::setprecision(2) << phase << ", noise = "
          << std::fixed << std::setprecision(2) << noise << ")";
    comparison_figure->title(title.str());
    // size of the window (width, height)
    comparison_figure->size(1200, 1200);
    // position of the window (x, y)
    comparison_figure->x_position(0);
    comparison_figure->y_position(0);
    // plot
    comparison_figure->add_subplot(3,2,0);
    matplot::title("Original Signal (Magnitude)");
    matplot::plot(x, plot_magnitude);
    comparison_figure->add_subplot(3,2, 1);
    matplot::title("Original Signal (Phase)");
    matplot::plot(x, plot_phase);

    comparison_figure->add_subplot(3,2, 2);
    matplot::title("Inverse FFT (Magnitude)");
    matplot::plot(x, plot_magnitude_ifft);
    comparison_figure->add_subplot(3,2, 3);
    matplot::title("Inverse FFT (Phase)");
    matplot::plot(x, plot_phase_ifft);

    comparison_figure->add_subplot(3,2, 4);
    matplot::title("FFT (Magnitude)");
    matplot::plot(x, plot_magnitude_fft);
    comparison_figure->add_subplot(3,2, 5);
    matplot::title("FFT (Phase)");
    matplot::plot(x, plot_phase_fft);

    comparison_figure->save(
        "examples/output/fft_signal_comparison_" +
        createReadableTimestamp("%Y%m%d_%H%M%S") +
        ".png"
    );
    comparison_figure->save(
        "examples/output/fft_signal_comparison_" +
        createReadableTimestamp("%Y%m%d_%H%M%S") +
        ".svg"
    );
    comparison_figure->show();
}


int main() {
    // ============================================= Configuration Loading =============================================
    // load the configuration from the sample file
    const std::string file_path = "examples/resources/fft_config.json";
    // load the configuration from the file
    const auto loader = new JSONConfigurationLoader();
    loader->loadConfigurationFromFile(file_path);
    const auto json_loaded = new JsonFieldHandler(loader->getConfigurationData());
    // get the simulation parameters
    const int signal_length = json_loaded->getSignalLength();
    const double *frequency = new double(json_loaded->getHzFrequency());
    const double *phase = new double(json_loaded->getPhase());
    const double *noise = new double(json_loaded->getNoise());
    const std::string *signal_domain = new std::string(json_loaded->getSignalDomain());
    // get the seed if it exists, otherwise set it to nullopt
    const std::optional<int> seed = json_loaded->hasSeed() ? std::optional(json_loaded->getSeed()) : std::nullopt;
    // prepare the signal saver and use the unique pointer to manage the memory
    const auto csv_signal_saver = std::make_unique<CsvSignalSaver>();
    // free unused memory
    delete loader;
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
    // and save it to a file
    csv_signal_saver->saveToFile(signal, "examples/output/input_signal");


    // ========================================== Sequential vs. Parallel FFT ==========================================
    printf("\n\nSequential vs. Parallel FFT\n");
    sequential_vs_parallel_fft(signal);


    // ====================================== Sequential vs. Parallel Inverse FFT ======================================
    printf("\n\nSequential vs. Parallel Inverse FFT\n");
    // pass the fft result as input
    std::vector<std::complex<double>> fft_res(signal_length);
    fft::solver::FastFourierTransform<1> tmp_solver(std::array{static_cast<size_t>(signal_length)});
    tmp_solver.compute(signal, fft_res, fft::solver::ComputationMode::OPENMP);
    sequential_vs_parallel_inverse_fft(fft_res);


    // =================================================== Plotting ===================================================
    plot_signal_fft_and_ifft(signal, *signal_domain, *frequency, *phase, *noise);

    return 0;
}
