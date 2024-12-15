# Fast Fourier Transform (FFT) - Group 20

[![OpenMP](https://img.shields.io/badge/OpenMP-5.2-blue.svg)](https://www.openmp.org/)

## Table of Contents

- [Description](#description)
- [How to run the code](#how-to-run-the-code)
- [Development Notes (to be removed)](#development-notes-to-be-removed)
  - [TODO](#todo)
  - [Utils](#utils)
    - [Generating a Random Time Domain Signal](#generating-a-random-time-domain-signal)
    - [Generating a Random Space Domain Signal](#generating-a-random-space-domain-signal)

------------------------------------------------------------------------------------------------------------------------

## Description

The repository contains several implementations of the 1-D Fast Fourier Transform (FFT) algorithm, primarily using the
[Cooley-Tukey algorithm][CK-FFT].

The main purpose of this repository is to provide:
- A sequential implementation of the Cooley-Tukey FFT algorithm using the iterative approach
  (rather than the classical recursive approach).
- A parallel implementation of the Cooley-Tukey FFT algorithm using the [OpenMP framework][OpenMP].
- A parallel implementation of the Cooler-Tukey FFT algorithm for the inverse FFT using the [OpenMP framework][OpenMP].

------------------------------------------------------------------------------------------------------------------------

## How to run the code

To run the code, you need to compile the code using the provided [CMakeLists.txt](CMakeLists.txt) file.

If you are a student at the [Politecnico di Milano][POLIMI],
you can easily use the [MK Library][MK library] (provided by the [MOX Laboratory][MOX Laboratory]) to compile the code.

In the CMakeLists.txt file, you can find the following lines that include the MK library:
```cmake
include_directories(
        /u/sw/toolchains/gcc-glibc/11.2.0/base/include
        # To include the Eigen library:
        # /u/sw/toolchains/gcc-glibc/11.2.0/pkgs/eigen/3.3.9/include/eigen3
        # To include the LIS library:
        # /u/sw/toolchains/gcc-glibc/11.2.0/pkgs/lis/2.0.30/include
)
```
If you don't want to use the MK library, you can comment out the lines containing the MK library.

If you have [CLion][CLion] installed, this is a simple story. Just open the project and run the code using the provided
[CMakeLists.txt](CMakeLists.txt) file. On the right side of the CLion window you can see the available executables.

Otherwise, you can compile the code using the command line.
1. Compile the CMakeFiles:
   ```bash
   cd 20-fft-20-fft # repository folder
   cmake . # where the CMakeLists.txt file is located
   ```
   After running the above command, you will see the Makefile in the repository folder.
   This Makefile contains the necessary commands to compile the code.
   Since the Makefile is generated automatically, you don't need to edit it.
   If you want to edit the Makefile, you can do so by modifying the file [CMakeLists.txt](CMakeLists.txt).
2. Compile all possible executables with the following command:
   ```bash
   # assuming you are in the repository folder where the CMakeLists.txt file is located
   make -f ./Makefile -C . all
   ```
3. And finally, run one of the compiled codes:
   ```bash
   # assuming you are in the repository folder where the CMakeLists.txt file is located
   ./main
   # and so on...
   ```
4. Clean the compiled files:
   ```bash
   # assuming you are in the repository folder where the CMakeLists.txt file is located
   make -f ./Makefile -C . clean
   ```

You can also compile just one of the challenges using the following commands:
1. Compile the CMakeFiles:
   ```bash
   cd 20-fft-20-fft # repository folder
   cmake .
   ```
2. Compile only one of the challenges using the following commands:
   ```bash
   # assuming you are in the repository folder where the CMakeLists.txt file is located
   make -f ./Makefile -C . main
   ```
3. And finally, run the compiled code:
   ```bash
   ./main
   ```

The names of the executables are available in the [CMakeLists.txt](CMakeLists.txt) file.
However, here is a list of available programs:
- [main](src/main.cpp)

------------------------------------------------------------------------------------------------------------------------

## Development Notes (to be removed)

### TODO

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


### Utils

#### Generating a Random Time Domain Signal

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

#### Generating a Random Space Domain Signal

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


[OpenMP]: https://www.openmp.org/
[POLIMI]: https://www.polimi.it/
[CLion]: https://www.jetbrains.com/clion/
[CK-FFT]: https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
[MOX Laboratory]: https://mox.polimi.it/
[MK library]: https://github.com/pcafrica/mk