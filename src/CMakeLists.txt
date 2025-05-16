add_library(
        signal_processing

        # config_loader
        config_loader/base_configuration_loader.hpp
        config_loader/json_configuration_loader.cpp
        config_loader/json_configuration_loader.hpp
        config_loader/json_field_handler.cpp
        config_loader/json_field_handler.hpp

        #discrete_cosine_transform
        discrete_cosine_transform/base_discrete_cosine_transform.hpp
        discrete_cosine_transform/base_discrete_cosine_transform.cpp
        discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.hpp
        discrete_cosine_transform/discrete_cosine_transform/discrete_cosine_transform.cpp
        discrete_cosine_transform/inverse_discrete_cosine_transform/inverse_discrete_cosine_transform.hpp
        discrete_cosine_transform/inverse_discrete_cosine_transform/inverse_discrete_cosine_transform.cpp
        discrete_cosine_transform/algorithms/dct.hpp
        discrete_cosine_transform/algorithms/dct.cpp
        discrete_cosine_transform/algorithms/dct_openmp.hpp
        discrete_cosine_transform/algorithms/dct_openmp.cpp
        discrete_cosine_transform/algorithms/idct.hpp
        discrete_cosine_transform/algorithms/idct.cpp
        discrete_cosine_transform/algorithms/idct_openmp.hpp
        discrete_cosine_transform/algorithms/idct_openmp.cpp

        # fourier_transform
        fourier_transform/base_fourier_transform.hpp
        # - algorithms
        fourier_transform/algorithms/cooley_tukey/cooley_tukey_fft.hpp
        fourier_transform/algorithms/cooley_tukey/cooley_tukey_fft.cpp
        fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_fft_openmp.hpp
        fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_fft_openmp.cpp
        fourier_transform/algorithms/cooley_tukey/cooley_tukey_inverse_fft.hpp
        fourier_transform/algorithms/cooley_tukey/cooley_tukey_inverse_fft.cpp
        fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_inverse_fft_openmp.hpp
        fourier_transform/algorithms/cooley_tukey/openmp/cooley_tukey_inverse_fft_openmp.cpp
        # - fast_fourier_transform
        fourier_transform/fast_fourier_transform/fast_fourier_transform.hpp
        # - inverse-fast_fourier_transform
        fourier_transform/inverse_fast_fourier_transform/inverse_fast_fourier_transform.hpp

        # jpeg-image-compression
        jpeg_image_compression/image/image.hpp
        jpeg_image_compression/image/image.cpp
        jpeg_image_compression/compressed_image/compressed_image.hpp
        jpeg_image_compression/compressed_image/compressed_image.cpp

        # rle-compressor
        rle_compressor/rle_compressor.hpp
        rle_compressor/rle_compressor.cpp

        # signal_generator
        signal_generator/base_signal_generator.hpp
        signal_generator/space_domain_signal_generator.cpp
        signal_generator/space_domain_signal_generator.hpp
        signal_generator/time_domain_signal_generator.cpp
        signal_generator/time_domain_signal_generator.hpp

        # signal_saver
        signal_saver/base_file_signal_saver.hpp
        signal_saver/csv_signal_saver.cpp
        signal_saver/csv_signal_saver.hpp

        # utils
        utils/timestamp.cpp
        utils/timestamp.hpp
        utils/bit_reversal.cpp
        utils/bit_reversal.hpp

        # zigzag_scan
        zigzag_scan/zigzag_scan.hpp
        zigzag_scan/zigzag_scan.cpp
)

if (HAS_CUDA)
    # cuda support
    add_library(
            signal_processing_cuda STATIC
            # - algorithms
            fourier_transform/algorithms/cooley_tukey/cuda/cooley_tukey_fft_cuda.cu
            fourier_transform/algorithms/cooley_tukey/cuda/cooley_tukey_fft_cuda.hpp
    )
    target_include_directories(signal_processing_cuda PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

target_include_directories(signal_processing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(signal_processing PUBLIC OpenMP::OpenMP_CXX)
