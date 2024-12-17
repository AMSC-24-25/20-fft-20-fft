# Fast Fourier Transform (FFT) - Group 20

[![OpenMP](https://img.shields.io/badge/OpenMP-5.2-blue.svg)](https://www.openmp.org/)

## Table of Contents

- [Description](#description)
- [How to run the code](#how-to-run-the-code)
  - [1. Write a JSON Configuration File](#1-write-a-json-configuration-file)
  - [2. Set the Environment Variable](#2-set-the-environment-variable)
  - [3. Compile and Run the Code](#3-compile-and-run-the-code)
- [Development Notes (to be removed)](#development-notes-to-be-removed)
  - [TODO](#todo)

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

### 1. Write a JSON Configuration File

To configure the simulation, you need to:

1. Create a JSON file with the following parameters:
    - `signal_length`. The length of the signal. Specifies the number of samples or data points in the signal.
    - `signal_domain`. The domain of the signal (time or space);
    - `hz_frequency`. Represents the frequency of the signal's oscillations. It depends on the signal domain.
                      For example, it refers to the number of cycles per second (hertz) in the time domain,
                      and it indicates spatial frequency (or the number of cycles per unit distance)
                      in the spatial domain.
    - `phase`. The phase of the signal.
               It allows you to move the signal along the time axis (i.e. to the left or right).
               It represents the initial angle of the sine or cosine function at time $t = 0$,
               or the shift of the signal waveform.
    - `noise`. Thanks to the noise parameter, the signal can be randomly distorted.
               In addition, the noise guarantees a more realistic signal generation.
    - `seed`. Seed for the random number generator (optional, if you want to make the simulation reproducible).
2. Set the environment variable to point to the JSON file.
   The name of the environment variable is `CONFIG_FILE_PATH_FFT`.
   If you don't set the environment variable,
   the simulation will use a sample configuration file: [sample-config.json](resources/sample-config.json);
   and the program will print a warning message.

In the [resources/json-schema](resources/json-schema) folder,
you can find the JSON schema that you can use to easily validate/write the JSON configuration file.

Unfortunately, the JSON schema is not used in the code yet because
we should install external libraries to validate the JSON file (not necessary at the moment).

If you are not familiar with JSON Schema, you can use the following website validator to validate your JSON file:
[JSON Schema Validator][JSONSchemaValidator].
You need to copy the [contents of the JSON schema file](resources/json-schema/json-schema.json) on the left
and the contents of your JSON file on the right (the validation is done automatically).

### 2. Set the Environment Variable

The simulation uses the environment variable `CONFIG_FILE_PATH_FFT` to read the JSON configuration file.

If it is not set, the simulation will use a sample configuration file:
[sample-config.json](resources/sample-config.json).

However, to set the environment variable, you can use the following command:
- On Linux:
  ```bash
  export CONFIG_FILE_PATH_FFT=/path/to/your/json/file.json
  ```
- On Windows:
  ```cmd
  set CONFIG_FILE_PATH_FFT=\path\to\your\json\file.json
  ```

The path to the JSON file can be absolute or relative to current working directory.

### 3. Compile and Run the Code

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
  - [x] Number of elements in the vector
  - [x] Seed for the random number generator (optional, if we want to make it reproducible)

- [x] Generate a vector: real values (complex?) as input.
  Can we use a seed for the random number generator to make it reproducible

- [ ] Implement the sequential FFT algorithm (how?)

- [ ] Implement the parallel FFT algorithm (how?)

- [ ] Implement the inverse FFT algorithm (how?)

- [ ] Return a vector.

- [ ] Output the results:
  - [ ] (optional) Plot the input signal and the output signal (https://github.com/lava/matplotlib-cpp)
  - [ ] Save the input signal and the output signal to a file


[OpenMP]: https://www.openmp.org/
[POLIMI]: https://www.polimi.it/
[CLion]: https://www.jetbrains.com/clion/
[CK-FFT]: https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
[MOX Laboratory]: https://mox.polimi.it/
[MK library]: https://github.com/pcafrica/mk
[JSONSchemaValidator]: https://www.jsonschemavalidator.net/