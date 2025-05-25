#include <iostream>
#include "config-loader/json-configuration-loader.hpp"
#include "matplot/matplot.h"
#include "signal-generator/space-domain-signal-generator.hpp"
#include "signal-generator/time-domain-signal-generator.hpp"
#include "signal-saver/csv-signal-saver.hpp"
#include "zigzag_scan/zigzag_scan.hpp"
#include "rle_compressor/rle_compressor.hpp"
#include "jpeg_image_compression/image/image.hpp"
#include "jpeg_image_compression/compressed_image/compressed_image.hpp"
#include "discrete-cosine-transform-solver/discrete-cosine-transform/discrete-cosine-transform.hpp"
#include "discrete-cosine-transform-solver/inverse-discrete-cosine-transform/inverse-discrete-cosine-transform.hpp"
#include <vector>

/**
 * Environment variable name for the file path for the configuration file.
 */
#define ENV_FILE_PATH "CONFIG_FILE_PATH_FFT"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

void print_vector(const std::vector<double>& vec) {
    for (int i=0; i<vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

void print_matrix(const std::vector<std::vector<double>>& mat) {
    for (int i=0; i<mat.size(); i++) {
        for (int j=0; j<mat[0].size(); j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ZIGZAG TEST
void zigzag_demo(){
    std::vector<std::vector<double>> matrix = {
        {1,1,1,4,5,6,7,0},
        {1,1,1,12,13,14,0,16},
        {1,18,19,20,21,0,23,24},
        {25,26,27,28,0,30,31,32},
        {33,34,35,0,37,38,39,40},
        {41,42,0,44,45,46,47,62},
        {49,0,51,52,53,54,62,56},
        {0,58,59,60,61,62,63,64}
    };

    std::vector<double> zz = ZigZagScan::scan(matrix);
    std::cout<<"\nOutput ZigZag scan: ";
    print_vector(zz);

    std::cout<<"\n\nOutput inverse ZigZag scan: "<<std::endl;
    std::vector<std::vector<double>> reverse_zz = ZigZagScan::inverse_scan(zz,8,8);
    print_matrix(reverse_zz);
}

// RLE_COMPRESSOR TEST
void rle_demo(){
    std::vector<double> vector = {2, 0, 0, 0, 0, 34, 17, 45, 45, 45, 12, 7, 9, 9, 9, 9, 9, 9, 5}
    std::vector<std::pair<int, int>> rle_vector = RLEcompressor::compress(vector);
    std::cout << "Output RLE compression: "<<std::endl;
    for(int i=0; i<rle_vector.size(); ++i){
        std::cout<<rle_vector[i].first<<" "<<rle_vector[i].second<<std::endl;
    }

    std::vector<double> rle_reverse = RLEcompressor::decompress(rle_vector);
    std::cout << "Output RLE compression: "<<std::endl;
    print_vector(rle_reverse);
}

// DCT TEST
void simple_dct_demo(){
    std::cout<<"\nSimple DCT example "<<std::endl;
    std::vector<double> vect = {1,2,3,4};
    std::cout<<"Input vector: ";
    print_vector(vect);

    // Compute the direct DCT in place and in parallel mode
    dct::solver::DiscreteCosineTransform dct_solver= dct::solver::DiscreteCosineTransform();
    dct::solver::ComputationMode cm = dct::solver::ComputationMode::OPENMP;
    dct_solver.compute(vect, cm);
    std::cout<<"Output DCT (parallel mode): ";
    print_vector(vect);

    // Compute the inverse DCT in place and in parallel mode
    dct::solver::InverseDiscreteCosineTransform idct_solver= dct::solver::InverseDiscreteCosineTransform();
    dct_solver.compute(vect, cm);
    std::cout<<"Output IDCT (parallel mode): ";
    print_vector(vect);
}

void jpeg_compression_demo(){
    // Create an Image object contining the image from image_path and save it as png
    const char* image_path = "/home/valentina/20-fft-20-fft/src/dog-bw.png";
    Image image = Image(image_path);
    std::string path = "/home/valentina/20-fft-20-fft/src/image.png";
    image.save_as_png(path);

    // Compress image and save it first in a binay file and then in a compressed binary file (ZIGZAG+RLE)
    CompressedImage compressed = image.compress();
    path = "/home/valentina/20-fft-20-fft/src/binary.bin";
    compressed.save_as_binary(path);
    path = "/home/valentina/20-fft-20-fft/src/compressed_binary.bin";
    compressed.save_as_compressed_binary(path);

    // Decompress the compressed image and save the result as a png
    Image decompressed = compressed.decompress();
    path = "/home/valentina/20-fft-20-fft/src/decompressed.png";
    decompressed.save_as_png(path);
}

void dct_demo(){
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
    std::vector<double> signal(signal_length);
    if (*signal_domain == "time") {
        // time domain
        printf("Generating time domain signal of length: %d.\n", signal_length);
        TimeDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generateReal1DSignal(signal_length, *frequency, *phase, *noise);
    } else {
        // space domain
        printf("Generating space domain signal of length: %d.\n", signal_length);
        SpaceDomainSignalGenerator domain_signal_generator(seed);
        signal = domain_signal_generator.generateReal1DSignal(signal_length, *frequency, *phase, *noise);
    }
    // free unused memory
    delete frequency;
    delete phase;
    delete noise;
    delete signal_domain;
    // and save it to a file
    csv_signal_saver->saveToFile(signal, "output/input_signal_dct");
    // since the transformation is in-place, we need to keep a copy of the original signal
    std::vector<double> original_signal = signal;
    std::vector<double> sequential_dct_input(signal_length);
    std::vector<double> parallel_dct_input(signal_length);
    std::vector<double> parallel_idct_input(signal_length);
    std::vector<double> sequential_idct_input(signal_length);
    for (size_t i = 0; i < signal_length; ++i) {
        original_signal[i] = signal[i];
        sequential_dct_input[i] = signal[i];
        parallel_dct_input[i] = signal[i];
        parallel_idct_input[i] = signal[i];
        sequential_idct_input[i] = signal[i];
    }

    const dct::solver::ComputationMode sequential = dct::solver::ComputationMode::SEQUENTIAL;
    const dct::solver::ComputationMode parallel = dct::solver::ComputationMode::OPENMP;

    dct::solver::DiscreteCosineTransform* dct_solver = new dct::solver::DiscreteCosineTransform();
    dct::solver::InverseDiscreteCosineTransform* idct_solver = new dct::solver::InverseDiscreteCosineTransform();

    // ================================================ Sequential DCT ================================================
    printf("\n\nDCT\n");
    const dct::solver::ComputationMode cm = dct::solver::ComputationMode::SEQUENTIAL;
    std::vector<double> sequential_dct_output(signal_length);
    dct_solver->compute(sequential_dct_input, sequential_dct_output, sequential);
    csv_signal_saver->saveToFile(sequential_dct_output, "output/dct_signal");

    // ================================================= Parallel FFT =================================================
    printf("\n\nParallel - DCT\n");
    std::vector<double> parallel_dct_output(signal_length);
    dct_solver->compute(parallel_dct_input, parallel_dct_output, parallel);
    csv_signal_saver->saveToFile(parallel_dct_output, "output/parallel_dct_signal");

    // ============================================ Sequential Inverse FFT ============================================
    printf("\n\nSequential - Inverse DCT\n");
    // Prepare the input of the IDCT by computing the DCT
    dct_solver->compute(sequential_idct_input, sequential);

    std::vector<double> sequential_idct_output(signal_length);
    // use the DCT result as input
    idct_solver->compute(sequential_idct_input, sequential_idct_output, sequential);
    csv_signal_saver->saveToFile(sequential_idct_output, "output/seq_inverse_dct_signal");

    // ============================================= Parallel Inverse FFT =============================================
    printf("\n\nParallel - Inverse DCT\n");
    // Prepare the input of the IDCT by computing the DCT
    dct_solver->compute(parallel_idct_input, parallel);

    std::vector<double> parallel_idct_output(signal_length);
    // use the DCT result as input
    idct_solver->compute(parallel_idct_input, parallel_idct_output, parallel);
    csv_signal_saver->saveToFile(parallel_idct_output, "output/parallel_inverse_dct_signal");

    // free the memory
    delete dct_solver;
    delete idct_solver;



    // =================================================== Plotting ===================================================

    // linear space for x-axis
    std::vector<double> x = matplot::linspace(0, signal_length - 1, signal_length);

    // create the plot for magnitude comparison
    auto magnitude_figure = matplot::figure();
    // title of the window
    magnitude_figure->name("Magnitude 1D DCT Signal Comparison");
    // title of the plot
    magnitude_figure->title("Magnitude Comparison");
    // size of the window (width, height)
    magnitude_figure->size(1300, 800);
    // position of the window (x, y)
    magnitude_figure->x_position(0);
    magnitude_figure->y_position(0);
    // plot
    magnitude_figure->add_subplot(3,1,1);
    matplot::plot(x, original_signal);
    matplot::title("Original Signal");
    magnitude_figure->add_subplot(3,1,2);
    matplot::plot(x, sequential_dct_output);
    matplot::title("Sequential DCT");
    magnitude_figure->add_subplot(3,1,3);
    matplot::plot(x, parallel_dct_output);
    matplot::title("Parallel DCT");
    // show the plot and block the execution
    magnitude_figure->show();

    // create the plot for original signal comparison
    auto comparison_idct_figure = matplot::figure();
    // title of the window
    comparison_idct_figure->name("Plot 1D FFT Signal Comparison");
    // title of the plot
    comparison_idct_figure->title("Original Signal vs. Inverse FFT Signal");
    // size of the window (width, height)
    comparison_idct_figure->size(1200, 800);
    // position of the window (x, y)
    comparison_idct_figure->x_position(0);
    comparison_idct_figure->y_position(0);
    // plot
    comparison_idct_figure->add_subplot(3,1,1);
    matplot::plot(x, original_signal);
    matplot::title("Original Signal");
    comparison_idct_figure->add_subplot(3,1,2);
    matplot::plot(x, sequential_idct_output);
    matplot::title("Sequential IDCT");
    comparison_idct_figure->add_subplot(3,1,3);
    matplot::plot(x, parallel_idct_output);
    matplot::title("Parallel IDCT");
    // show the plot and block the execution
    comparison_idct_figure->show();
}

int main() {
    //simple_dct_demo();
    //zigzag_demo();
    //rle_compression_demo();
    jpeg_compression_demo();
    dct_demo();

    return 0;
}

