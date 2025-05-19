#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <complex>
#include <functional>
#include <optional>
#include <random>

/**
 * Generates a vector of powers of two from 2^1 to 2^max_pow.
 *
 * Note: The first element (2^0) is not included in the returned vector.
 * @param max_pow The maximum power of two to generate. Default is 22, then 4'194'304.
 * @return A vector containing the powers of two.
 */
inline std::vector<size_t> generatePowersOfTwo(const size_t max_pow = 22) {
    // cache the powers of two to avoid recomputing them for each call
    static std::unordered_map<size_t, std::vector<size_t>> cache;
    if (const auto it = cache.find(max_pow); it != cache.end()) {
        return it->second;
    }
    // generate the powers of two from 2^1 to 2^max_pow
    std::vector<size_t> powers;
    for (size_t i = 1; i <= max_pow; ++i) {
        // 2^i is equivalent to shifting 1 left by i bits
        // e.g., 2^3 = 1 << 3 = 8 (0...0001000)
        // 2^4 = 1 << 4 = 16 (0...00010000)
        // 2^5 = 1 << 5 = 32 (0...00100000)
        powers.push_back(1 << i);
    }
    cache[max_pow] = powers;
    return powers;
}

/**
 * Generates a random input vector of complex numbers.
 *
 * It flattens the input dimensions into a single vector of complex numbers.
 * So if the input is 2x3x4, it will create a vector of size 24 (2*3*4).
 *
 * @tparam N The number of dimensions.
 * @param dims The dimensions of the input vector.
 * @param seed The seed for the random number generator (default is 42).
 * @return A vector of complex numbers with the specified dimensions.
 */
template <size_t N>
std::vector<std::complex<double>> generateInput(const std::array<size_t, N>& dims, const size_t seed = 42) {
    // calculate the total size of the input vector
    // dims is an array of size N, so we need to multiply all its elements
    // to get the total size
    size_t total = 1;
    for (const size_t d : dims) total *= d;

    // use a random number generator to fill the vector with random complex numbers
    // use a fixed seed for reproducibility
    std::mt19937 gen(seed);
    // uniform distribution in the range [-1.0, 1.0]
    std::uniform_real_distribution dist(-1.0, 1.0);

    // create a vector of complex numbers with the specified size
    std::vector<std::complex<double>> data(total);
    for (auto& x : data)
        x = {dist(gen), dist(gen)};
    return data;
}

/**
 * Generates all valid shapes for a given number of dimensions.
 *
 * A valid shape is defined as a combination of powers of two that do not exceed
 * the specified maximum total size.
 *
 * The function uses backtracking to explore all combinations of powers of two
 * for N dimensions, ensuring that the product of the dimensions does not exceed
 * the maximum total size.
 *
 * @tparam N The number of dimensions.
 * @param max_total_size The maximum total size for the shapes.
 * @param min The minimum value for the dimensions (optional).
 * @return A vector of arrays representing the valid shapes.
 */
template <size_t N>
std::vector<std::array<size_t, N>> generateValidShapes(
    const size_t max_total_size,
    const std::optional<size_t> min = std::nullopt
) {
    std::vector<std::array<size_t, N>> results;
    // generate the powers of two up to the maximum total size
    const std::vector<size_t> powers = generatePowersOfTwo();

    std::array<size_t, N> current;
    /**
     * Backtracking function to generate all combinations of powers of two.
     *
     * Backtracking is a recursive search that builds a solution step-by-step,
     * abandoning any path that cannot possibly lead to a valid solution.
     *
     * More specifically, it explores all combinations of powers of two
     * for N dimensions, ensuring that the product of the dimensions
     * does not exceed the maximum total size.
     *
     * In general, Backtracking is a general algorithmic technique for:
     * 1. Building candidates for a solution incrementally;
     * 2. Abandoning ("backtracking") a candidate
     *    as soon as it's clear it's invalid or won’t lead to a full solution.
     *
     * In this case, it:
     * 1. Goes "depth" levels deep (from 0 to N-1);
     * 2. Maintains the current product of dimensions.
     * It explores a tree of possible values,
     * trying all combinations of powers of two across N dimensions,
     * but cuts off branches when the product exceeds max_total_size.
     *
     * @param depth The current depth in the recursion (dimension index).
     * @param product The current product of the dimensions.
     */
    std::function<void(size_t, size_t)> backtrack = [&](size_t depth, const size_t product) {
        // base case: filled all N dimensions
        if (depth == N) {
            // current array now represents one valid shape (e.g., {256, 32, 2})
            // store it in the results
            results.push_back(current);
            return;
        }
        // recursive case: iterate over all powers of two
        for (size_t p : powers) {
            // skip values minus of min
            if (min.has_value() && p < min) continue;
            // check if the product exceeds the maximum total size
            // if it does, break the loop to avoid unnecessary calculations
            if (product * p > max_total_size) break;
            // try placing power p at dimension depth
            // (if adding it would exceed max_total_size, prev. step,
            // stop, because all future powers are larger)
            current[depth] = p;
            // recursively call backtrack for the next dimension
            // (product is updated to include the new dimension)
            // (e.g., if current is {256, 32, 2} and p is 4,
            // product becomes 256 * 32 * 2 * 4)
            backtrack(depth + 1, product * p);
        }
    };
    // start backtracking from the first dimension (0) and initial product of 1
    // (1 is the multiplicative identity, so it doesn't affect the product)
    backtrack(0, 1);
    return results;
}

/**
 * Retrieves the value of a command-line argument.
 *
 * The argument should be in the format "-key=value".
 * If the key is found, the value is returned; otherwise, std::nullopt is returned.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @param key The key to search for (without the leading '-').
 * @param double_dash Whether to use "--" instead of "-" for the key.
 * @return An optional string containing the value if found, or std::nullopt if not found.
 */
inline std::optional<std::string> getArgValue(
    const int argc,
    char** argv,
    const std::string& key,
    const bool double_dash = false
) {
    const std::string prefix = (double_dash ? "--" : "-") + key + "=";
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg.find(prefix) == 0) {
            return arg.substr(prefix.size());
        }
    }
    return std::nullopt;
}


#endif // UTILS_HPP
