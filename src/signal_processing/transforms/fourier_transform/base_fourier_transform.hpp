#ifndef BASE_FOURIER_TRANSFORM
#define BASE_FOURIER_TRANSFORM
#include <complex>
#include <vector>
#include <numeric>
#include <functional>
#include <optional>
#include <omp.h>

namespace signal_processing::fft::solver
{
    /**
     * Enumeration for the modes of computation.
     *
     * The modes are:
     *  - <code>SEQUENTIAL</code>: Sequential computation.
     *  - <code>OPENMP</code>: Parallel computation using OpenMP.
     */
    enum class ComputationMode {
        SEQUENTIAL,
        OPENMP
    };

    /**
     * Abstract template class for Fourier Transform solvers.
     *
     * This class provides an interface for computing the Fourier Transform
     * of a given input vector. It supports different modes of computation,
     * including sequential and parallel (OpenMP) modes.
     */
    template <size_t N>
    class BaseFourierTransform {
    public:
        static_assert(
            N > 0,
            "The number of dimensions N must be greater than 0."
        );

        /**
         * The dimensions of the input vector.
         * This is a fixed-size array of size N,
         * where each element represents the size of the corresponding dimension.
         *
         * For example, for a 2D FFT,
         * dimensions[0] would be the number of rows and dimensions[1] would be the number of columns.
         *
         * @note The number of dimensions is equal to the template parameter N.
         * @warning The dimensions must be powers of 2 for the Cooley-Tukey FFT algorithm to work correctly.
         */
        const std::array<size_t, N> dims;

        /**
         * Create a Fourier Transform solver.
         *
         * @param dimensions An array of dimensions for the Fourier Transform.
         * @throws std::invalid_argument if the number of dimensions is not equal to N.
         * @throws std::invalid_argument if the dimensions are not powers of 2.
         */
        explicit BaseFourierTransform(const std::array<size_t, N>& dimensions): dims(dimensions) {
            if (dimensions.size() != N) {
                throw std::invalid_argument(
                    "The number of dimensions must match the template parameter N."
                );
            }
            // check if all dimensions are positive powers of 2
            // (i.e., greater than 0 and a power of 2)
            // static_assert is not used here because it would require the dimensions to be known at compile time
            // and the dimensions are passed at runtime
            for (const size_t &dim : dimensions) {
                if (dim <= 0 || (dim & dim - 1) != 0) {
                    throw std::invalid_argument(
                        "All dimensions must be positive powers of 2."
                    );
                }
            }
        }

        virtual ~BaseFourierTransform() = default;

        /**
         * Compute the Fourier Transform of the input vector (in-place).
         *
         * The input vector will be modified to store the result of the transformation.
         *
         * @param input The input vector to be transformed.
         * @param mode The mode of computation.
         * @param threads The number of CPU threads to use for parallel computation (if applicable).
         *                If not specified, the default number of threads will be used.
         * @throws std::invalid_argument if the input vector size does not match the expected size (based on dimensions).
         */
        void compute(
            std::vector<std::complex<double>> &input,
            const ComputationMode mode,
            const std::optional<int> threads = std::nullopt
        ) {
            // check if the size of the input vector corresponds to the multiplication of each element of dims
            const size_t expected_size = std::accumulate(
                dims.begin(), dims.end(), 1, std::multiplies()
            );
            if (input.size() != expected_size) {
                throw std::invalid_argument(
                    "Input vector size does not match the expected size based on dimensions. Given: " +
                    std::to_string(input.size()) + ", Expected: " +
                    std::to_string(expected_size)
                );
            }
            if (mode == ComputationMode::SEQUENTIAL) {
		        // save the number of threads before calling the OpenMP function
		        const int numThreads = omp_get_max_threads();
		        // force sequential mode (1 thread)
		        omp_set_num_threads(1);
                // print how many threads are used
                this->computeND(input, this->getSequentialTransform());
		        // restore number of threads
		        omp_set_num_threads(numThreads);
            } else if (mode == ComputationMode::OPENMP) {
                // save the number of threads before calling the OpenMP function
                const int numThreads = omp_get_max_threads();
                // if numberOfThreads is specified, set the number of threads
                if (threads.has_value()) {
                    omp_set_num_threads(threads.value());
                }
                // compute the Fourier Transform using OpenMP
                this->computeND(input, this->getOpenMPTransform());
                // restore the number of threads to the original value;
                // note: If numberOfThreads is not specified,
                //       it is not a problem because the same value will be set again
                omp_set_num_threads(numThreads);
            } else {
                throw std::invalid_argument("Invalid mode specified.");
            }
        }

        /**
         * Compute the Fourier Transform of the input vector and store the result in the output vector.
         *
         * The input vector will not be modified.
         *
         * @param input The input vector to be transformed.
         * @param output The output vector after transformation.
         * @param mode The mode of computation.
         * @param threads The number of CPU threads to use for parallel computation (if applicable).
         *                If not specified, the default number of threads will be used.
         */
        void compute(
            const std::vector<std::complex<double>> &input,
            std::vector<std::complex<double>> &output,
            const ComputationMode mode,
            const std::optional<int> threads = std::nullopt
        ) {
            output = input;
            this->compute(output, mode, threads);
        }

    protected:
        /**
         * Transform function type.
         *
         * This is a function that takes a vector of complex numbers and applies the Fourier Transform.
         */
        using transform_t = std::function<void(std::vector<std::complex<double>>&)>;

        /**
         * Compute the Fourier Transform of the input vector in N dimensions.
         *
         * This method is responsible for iterating over each dimension and applying the
         * transform function to the corresponding slices of the input vector.
         *
         * @param input The input vector to be transformed.
         * @param transform The transform function to be applied.
         */
        void computeND(
            std::vector<std::complex<double>>& input,
            const transform_t& transform
        ) {
            // dimensions of the N-dimensional space
            const std::array<size_t, N> dims = this->dims;
            /**
             * Strides for each dimension (used for flattening the N-dimensional array).
             *
             * The calculation of strides is necessary to map multi-dimensional coordinates to a flat,
             * one-dimensional index in memory.
             *
             * This is because multi-dimensional arrays are stored in a contiguous block of memory,
             * and strides help determine the memory offset for each dimension.
             *
             * For example, in a 2D array with dimensions [rows, columns],
             * the stride for the last dimension (columns) is 1 because
             * elements in this dimension are stored contiguously.
             * The stride for the first dimension (rows) is equal to the number of columns,
             * as moving to the next row requires skipping all the columns of the current row.
             *
             * The formula for strides ensures that the correct memory location
             * is accessed for any given multi-dimensional coordinate.
             *
             * It is a common pattern in numerical computing and is used in libraries like NumPy.
             */
            const std::array<size_t, N> strides = this->computeStrides(dims);
            // total number of elements in the input vector
            const size_t totalSize = input.size();

            // iterate over each dimension (axis)
            for (size_t axis = 0; axis < N; ++axis) {
                // current axis size
                const size_t axisSize = dims[axis];
                // total number of slices along the current axis,
                // so how many times we need to apply the transform function
                const size_t numSlices = totalSize / axisSize;

                // parallelize the loop over slices if OpenMP is enabled
                // but only if the number of slices is greater than 1
                // this is to avoid overhead of parallelization for small arrays
                # pragma omp parallel for if(numSlices > 1)
                for (size_t slice = 0; slice < numSlices; ++slice) {
                    // prepare the coordinates for the current slice
                    std::vector<std::complex<double>> line(axisSize);
                    std::array<size_t, N> baseCoord{};
                    size_t temp = slice;
                    // fill the baseCoord array with the coordinates of the current slice
                    for (int i = N - 1; i >= 0; --i) {
                        if (i != static_cast<int>(axis)) {
                            baseCoord[i] = temp % dims[i];
                            temp /= dims[i];
                        }
                    }
                    // fill the line vector with the values from the input vector
                    for (size_t i = 0; i < axisSize; ++i) {
                        baseCoord[axis] = i;
                        line[i] = input[this->toFlatIndex(baseCoord, strides)];
                    }

                    // apply the transform function to the line vector
                    transform(line);

                    // store the transformed values back into the input vector
                    for (size_t i = 0; i < axisSize; ++i) {
                        baseCoord[axis] = i;
                        // compute the flat index for the transformed value
                        // because the baseCoord array has been modified
                        input[this->toFlatIndex(baseCoord, strides)] = line[i];
                    }
                }
            }
        }

        /**
         * Compute the strides for each dimension.
         *
         * Strides are used to map multidimensional coordinates to a flat, one-dimensional index.
         *
         * @param dims The dimensions of the N-dimensional space.
         * @return An array of strides for each dimension.
         */
        std::array<size_t, N> computeStrides(const std::array<size_t, N>& dims) {
            // computed strides for each dimension
            std::array<size_t, N> strides;
            // the last dimension's stride is always 1 because it represents the smallest step in memory
            strides[N - 1] = 1;
            // iterate backward through the dimesions to compute the strides
            for (int i = static_cast<int>(N) - 2; i >= 0; --i)
                strides[i] = strides[i + 1] * dims[i + 1];
            return strides;
        }

        /**
         * Convert multidimensional coordinates to a flat index.
         *
         * This method computes the flat index in a one-dimensional array
         * corresponding to the given multidimensional coordinates and strides.
         *
         * @param coord The multi-dimensional coordinates.
         * @param strides The strides for each dimension.
         * @return The flat index in the one-dimensional array.
         */
        size_t toFlatIndex(const std::array<size_t, N>& coord, const std::array<size_t, N>& strides) {
            size_t flat = 0;
            for (size_t i = 0; i < N; ++i)
                flat += coord[i] * strides[i];
            return flat;
        }

        /**
         * Convert a slice index to multidimensional coordinates.
         *
         * This method computes the multidimensional coordinates corresponding
         * to the given slice index and axis.
         *
         * @param slice The slice index.
         * @param dims The dimensions of the N-dimensional space.
         * @param axis The axis along which the slice is taken.
         * @param axisIndex The index along the specified axis.
         * @return The multidimensional coordinates corresponding to the slice index.
         */
        std::array<size_t, N> sliceToCoord(
            size_t slice,
            const std::array<size_t, N>& dims,
            const size_t axis,
            size_t axisIndex
        ) {
            std::array<size_t, N> coord{};
            // similar to the toFlatIndex method, but in reverse;
            // we need to set the axis index to the specified value
            // and compute the other coordinates based on the slice index
            for (int i = N - 1; i >= 0; --i) {
                if (i == static_cast<int>(axis)) {
                    coord[i] = axisIndex;
                } else {
                    coord[i] = slice % dims[i];
                    slice /= dims[i];
                }
            }
            return coord;
        }

        /**
         * Get the sequential transform function.
         *
         * This method should be overridden by derived classes to provide the specific
         * implementation of the Fourier Transform.
         *
         * @return The sequential transform function.
         */
        [[nodiscard]] virtual transform_t getSequentialTransform() const = 0;

        /**
         * Get the OpenMP transform function.
         *
         * This method should be overridden by derived classes to provide the specific
         * implementation of the Fourier Transform.
         *
         * @return The OpenMP transform function.
         */
        [[nodiscard]] virtual transform_t getOpenMPTransform() const = 0;
    };
}


#endif //BASE_FOURIER_TRANSFORM
