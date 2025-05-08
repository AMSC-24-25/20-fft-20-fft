import numpy as np
import subprocess
import os

def write_complex_input(filename, data):
    with open(filename, "w") as f:
        f.write(f"{len(data)}\n")
        for c in data:
            f.write(f"{c.real} {c.imag}\n")

def read_complex_output(filename):
    output = []
    with open(filename, "r") as f:
        for line in f:
            real, imag = map(float, line.strip().split())
            output.append(complex(real, imag))
    return np.array(output)

def test_fft_openmp_against_numpy():
    N = 8
    np.random.seed(42)
    input_data = np.random.rand(N) + 1j * np.random.rand(N)

    write_complex_input("input.txt", input_data)

    compile_result = subprocess.run(
        [
            "g++",
            "main_fft_openmp.cpp",
            "cooley-tukey-fft-openmp.cpp",
            "../../../utils/bit-reversal.cpp",
            "-I../../..",
            "-fopenmp",
            "-o", "fft_openmp_exec",
            "-std=c++17",
            "-lm"
        ]
    )
    assert compile_result.returncode == 0, "Compilazione FFT OpenMP fallita"

    run_result = subprocess.run(["./fft_openmp_exec"])
    assert run_result.returncode == 0, "Esecuzione FFT OpenMP fallita"

    cpp_output = read_complex_output("output.txt")
    numpy_output = np.fft.fft(input_data)

    np.testing.assert_allclose(cpp_output, numpy_output, rtol=1e-5, atol=1e-8)

    os.remove("input.txt")
    os.remove("output.txt")
    os.remove("fft_openmp_exec")
