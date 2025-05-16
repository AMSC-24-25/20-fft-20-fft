add_library(
        signal_processing

        # config-loader
        config-loader/abstract-configuration-loader.hpp
        config-loader/json-configuration-loader.cpp
        config-loader/json-configuration-loader.hpp
        config-loader/json-field-handler.cpp
        config-loader/json-field-handler.hpp

        # fourier-transform-solver
        fourier-transform-solver/fourier-transform-solver.hpp
        # - algorithms
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft.hpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft.cpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft-openmp.hpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-fft-openmp.cpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft.hpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft.cpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft-openmp.hpp
        fourier-transform-solver/algorithms/cooley-tukey/cooley-tukey-inverse-fft-openmp.cpp
        # - fast-fourier-transform
        fourier-transform-solver/fast-fourier-transform/fast-fourier-transform.hpp
        # - inverse-fast-fourier-transform
        fourier-transform-solver/inverse-fast-fourier-transform/inverse-fast-fourier-transform.hpp

        # signal-generator
        signal-generator/abstract-signal-generator.hpp
        signal-generator/space-domain-signal-generator.cpp
        signal-generator/space-domain-signal-generator.hpp
        signal-generator/time-domain-signal-generator.cpp
        signal-generator/time-domain-signal-generator.hpp

        # signal-saver
        signal-saver/abstract-file-signal-saver.hpp
        signal-saver/csv-signal-saver.cpp
        signal-saver/csv-signal-saver.hpp

        # utils
        utils/timestamp.cpp
        utils/timestamp.hpp
        utils/bit-reversal.cpp
        utils/bit-reversal.hpp

        #discrete-cosine-transform-solver
        discrete-cosine-transform-solver/discrete-cosine-transform-solver.hpp
        discrete-cosine-transform-solver/discrete-cosine-transform-solver.cpp
        discrete-cosine-transform-solver/discrete-cosine-transform/discrete-cosine-transform.hpp
        discrete-cosine-transform-solver/discrete-cosine-transform/discrete-cosine-transform.cpp
        discrete-cosine-transform-solver/inverse-discrete-cosine-transform/inverse-discrete-cosine-transform.hpp
        discrete-cosine-transform-solver/inverse-discrete-cosine-transform/inverse-discrete-cosine-transform.cpp
        discrete-cosine-transform-solver/algorithms/dct.hpp
        discrete-cosine-transform-solver/algorithms/dct.cpp
        discrete-cosine-transform-solver/algorithms/dct_openMP.hpp
        discrete-cosine-transform-solver/algorithms/dct_openMP.cpp
        discrete-cosine-transform-solver/algorithms/idct.hpp
        discrete-cosine-transform-solver/algorithms/idct.cpp
        discrete-cosine-transform-solver/algorithms/idct_openMP.hpp
        discrete-cosine-transform-solver/algorithms/idct_openMP.cpp

        # zigzag-scan
        zigzag_scan/zigzag_scan.hpp
        zigzag_scan/zigzag_scan.cpp

        # rle-compressor
        rle_compressor/rle_compressor.hpp
        rle_compressor/rle_compressor.cpp

        # jpeg-image-compression
        jpeg_image_compression/image/image.hpp
        jpeg_image_compression/image/image.cpp
        jpeg_image_compression/compressed_image/compressed_image.hpp
        jpeg_image_compression/compressed_image/compressed_image.cpp
)

target_include_directories(signal_processing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(signal_processing PUBLIC OpenMP::OpenMP_CXX)
