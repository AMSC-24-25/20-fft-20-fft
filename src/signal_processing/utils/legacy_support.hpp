#ifndef LEGACY_SUPPORT_HPP
#define LEGACY_SUPPORT_HPP

/**
 * Legacy Support Module
 *
 * This module provides support for legacy C++ code.
 * The library is designed to be compatible with older C++ standards and practices.
 *
 * This class includes utilities found in newer C++ standards.
 * However, they are provided here for compatibility with legacy code.
 */
namespace sp::utils::legacy {
    /**
     * Optional class is a simple implementation of std::optional.
     * It is used to represent an optional value that may or may not be present.
     *
     * This class is provided for compatibility with legacy code that does not use C++17 or later.
     */
    template<typename T>
    class Optional {
        /**
         * Indicates whether the Optional has a value.
         * If has_value_ is true, value_ contains a valid value.
         */
        bool has_value_;
        /**
         * The value contained in the Optional.
         * If has_value_ is false, this value is not valid.
         */
        T value_;
    public:
        /**
         * Default constructor initializes the Optional to an empty state.
         * has_value_ is set to false, and value_ is default-constructed.
         */
        Optional() : has_value_(false), value_() {}
        /**
         * Constructor that initializes the Optional with a value.
         * has_value_ is set to true, and value_ is initialized with the provided value.
         * @param value The value to initialize the Optional with.
         */
        explicit Optional(const T& value) : has_value_(true), value_(value) {}

        /**
         * Checks if the Optional has a value.
         * @return true if the Optional has a value, false otherwise.
         */
        bool has_value() const {
            return has_value_;
        }

        /**
         * Returns the value contained in the Optional.
         * @return const reference to the value if has_value_ is true.
         */
        const T& value() const { return value_; }

        /**
         * Returns a reference to the value contained in the Optional.
         * This function is used to modify the value if has_value_ is true.
         * @return reference to the value if has_value_ is true.
         */
        T& value() { return value_; }

        const T& operator*() const { return value_; }
        T& operator*() { return value_; }

        /**
         * Enables the use of the Optional in boolean contexts.
         */
        explicit operator bool() const { return has_value_; }
    };
} // namespace sp::utils::legacy

#endif //LEGACY_SUPPORT_HPP
