#ifndef SIGNAL_PROCESSING_HPP
#define SIGNAL_PROCESSING_HPP

// compression
#include <compression/image_compression_haar_wavelet/image_compression_haar_wavelet.hpp>
#include <compression/jpeg_image_compression/compressed_image/compressed_image.hpp>
#include <compression/jpeg_image_compression/image/image.hpp>

// handlers
#include <handlers/config_loader/base_configuration_loader.hpp>
#include <handlers/config_loader/json_configuration_loader.hpp>
#include <handlers/config_loader/json_field_handler.hpp>
#include <handlers/signal_generator/base_signal_generator.hpp>
#include <handlers/signal_generator/space_domain_signal_generator.hpp>
#include <handlers/signal_generator/time_domain_signal_generator.hpp>
#include <handlers/signal_saver/base_file_signal_saver.hpp>
#include <handlers/signal_saver/csv_signal_saver.hpp>

// transforms
#include <transforms/discrete_cosine_transform/base_discrete_cosine_transform.hpp>
#include <transforms/discrete_cosine_transform/algorithms/dct.hpp>
#include <transforms/discrete_cosine_transform/algorithms/dct_openmp.hpp>
#include <transforms/discrete_cosine_transform/algorithms/idct.hpp>
#include <transforms/discrete_cosine_transform/algorithms/idct_openmp.hpp>
#include <transforms/discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.hpp>
#include <transforms/discrete_cosine_transform/inverse_discrete_cosine_transform/inverse_discrete_cosine_transform.hpp>
#include <transforms/fourier_transform/base_fourier_transform.hpp>
#include <transforms/fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_fft_openmp.hpp>
#include <transforms/fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_inverse_fft_openmp.hpp>
#include <transforms/fourier_transform/algorithms/cooley_tukey/cooley_tukey_fft.hpp>
#include <transforms/fourier_transform/algorithms/cooley_tukey/cooley_tukey_inverse_fft.hpp>
#include <transforms/fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp>
#include <transforms/fourier_transform/inverse_fast_fourier_transform/inverse_fast_fourier_transform.hpp>
#include <transforms/haar_wavelet_transform/haar_wavelet_1d.hpp>
#include <transforms/haar_wavelet_transform/haar_wavelet_2d.hpp>

// utils
#include <utils/bit_reversal.hpp>
#include <utils/rle_compressor.hpp>
#include <utils/timestamp.hpp>
#include <utils/zigzag_scan.hpp>

#endif //SIGNAL_PROCESSING_HPP
