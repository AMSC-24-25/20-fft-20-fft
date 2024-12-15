# FFT
Fast fourier transform

## TODO

- Environment variables or, better, a configuration file to set the following parameters
  ([json](https://github.com/nlohmann/json?tab=readme-ov-file)): 
  - [ ] Number of elements in the vector
  - [ ] Real or complex values simulation
  - [ ] Seed for the random number generator (optional, if we want to make it reproducible)

- [ ] Generate a vector: real values (complex?) as input.
  Can we use a seed for the random number generator to make it reproducible

- [ ] Implement the FFT algorithm (how?)

- [ ] Implement the inverse FFT algorithm (how?)

- [ ] Return a vector.

- [ ] Output the results:
  - [ ] (optional) Plot the input signal and the output signal (https://github.com/lava/matplotlib-cpp)
  - [ ] Save the input signal and the output signal to a file


## Utils

### Generating a Random Time Domain Signal

```cpp
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

int main() {
    // Default random engine
    std::default_random_engine engine;
    
    // Seed the engine (if necessary hardcoded seed)
    engine.seed(123);
    
    // Otherwise if we want a random seed
    std::random_device rd;
    std::default_random_engine eng(rd());
    
    // Create a uniform distribution between 0 and 1
    std::uniform_real_distribution<> distr(0, 1);

    // Create a signal with 100 random values
    std::vector<double> signal(100);
    for (auto& value : signal) {
        value = distr(eng);
    }

    // Print
    for (const auto& value : signal) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Generating a Random Space Domain Signal

```cpp
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

int main() {
    // Random generation: ...
    // Same as before

    // Generate a random space domain signal
    std::vector<std::vector<double>> signal(10, std::vector<double>(10));
    for (auto& row : signal) {
        for (auto& value : row) {
            value = distr(eng);
        }
    }

    // Print
    for (const auto& row : signal) {
        for (const auto& value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
```
