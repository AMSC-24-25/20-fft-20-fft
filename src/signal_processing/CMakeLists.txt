add_library(
        signal_processing

        # jpeg-image-compression
        compression/jpeg_image_compression/image/image.hpp
        compression/jpeg_image_compression/image/image.cpp
        compression/jpeg_image_compression/compressed_image/compressed_image.hpp
        compression/jpeg_image_compression/compressed_image/compressed_image.cpp
        compression/image_compression_haar_wavelet/image_compression_haar_wavelet.cpp

        # config_loader
        handlers/config_loader/base_configuration_loader.hpp
        handlers/config_loader/json_configuration_loader.cpp
        handlers/config_loader/json_configuration_loader.hpp
        handlers/config_loader/json_field_handler.cpp
        handlers/config_loader/json_field_handler.hpp
        # signal_generator
        handlers/signal_generator/base_signal_generator.hpp
        handlers/signal_generator/space_domain_signal_generator.cpp
        handlers/signal_generator/space_domain_signal_generator.hpp
        handlers/signal_generator/time_domain_signal_generator.cpp
        handlers/signal_generator/time_domain_signal_generator.hpp

        # signal_saver
        handlers/signal_saver/base_file_signal_saver.hpp
        handlers/signal_saver/csv_signal_saver.cpp
        handlers/signal_saver/csv_signal_saver.hpp

        #discrete_cosine_transform
        transforms/discrete_cosine_transform/base_discrete_cosine_transform.hpp
        transforms/discrete_cosine_transform/base_discrete_cosine_transform.cpp
        transforms/discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.hpp
        transforms/discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.cpp
        transforms/discrete_cosine_transform/inverse_discrete_cosine_transform/inverse_discrete_cosine_transform.hpp
        transforms/discrete_cosine_transform/inverse_discrete_cosine_transform/inverse_discrete_cosine_transform.cpp
        transforms/discrete_cosine_transform/algorithms/dct.hpp
        transforms/discrete_cosine_transform/algorithms/dct.cpp
        transforms/discrete_cosine_transform/algorithms/dct_openmp.hpp
        transforms/discrete_cosine_transform/algorithms/dct_openmp.cpp
        transforms/discrete_cosine_transform/algorithms/idct.hpp
        transforms/discrete_cosine_transform/algorithms/idct.cpp
        transforms/discrete_cosine_transform/algorithms/idct_openmp.hpp
        transforms/discrete_cosine_transform/algorithms/idct_openmp.cpp

        # fourier_transform
        transforms/fourier_transform/base_fourier_transform.hpp
        # - algorithms
        transforms/fourier_transform/algorithms/cooley_tukey/cooley_tukey_fft.hpp
        transforms/fourier_transform/algorithms/cooley_tukey/cooley_tukey_fft.cpp
        transforms/fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_fft_openmp.hpp
        transforms/fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_fft_openmp.cpp
        transforms/fourier_transform/algorithms/cooley_tukey/cooley_tukey_inverse_fft.hpp
        transforms/fourier_transform/algorithms/cooley_tukey/cooley_tukey_inverse_fft.cpp
        transforms/fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_inverse_fft_openmp.hpp
        transforms/fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_inverse_fft_openmp.cpp
        # - fast_fourier_transform
        transforms/fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp
        # - inverse-fast_fourier_transform
        transforms/fourier_transform/inverse_fast_fourier_transform/inverse_fast_fourier_transform.hpp

        # haar_wavelet_transform
        transforms/haar_wavelet_transform/haar_wavelet_1d.cpp
        transforms/haar_wavelet_transform/haar_wavelet_2d.cpp

        # utils
        utils/bit_reversal.cpp
        utils/bit_reversal.hpp
        utils/rle_compressor.hpp
        utils/rle_compressor.cpp
        utils/timestamp.cpp
        utils/timestamp.hpp
        utils/zigzag_scan.hpp
        utils/zigzag_scan.cpp
)

if (HAS_CUDA)
    # cuda support
    add_library(
            signal_processing_cuda STATIC
            # - algorithms
            transforms/fourier_transform/algorithms/cooley_tukey/cuda/cooley_tukey_fft_cuda.cu
            transforms/fourier_transform/algorithms/cooley_tukey/cuda/cooley_tukey_fft_cuda.hpp
    )
    target_include_directories(signal_processing_cuda PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

target_include_directories(signal_processing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(signal_processing PUBLIC OpenMP::OpenMP_CXX)
