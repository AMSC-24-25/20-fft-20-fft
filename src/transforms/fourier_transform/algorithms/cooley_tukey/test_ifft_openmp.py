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

def test_ifft_openmp_against_numpy():
    N = 8
    np.random.seed(123)
    freq_data = np.random.rand(N) + 1j * np.random.rand(N)

    write_complex_input("input.txt", freq_data)

    compile_result = subprocess.run(
        [
            "g++",
            "main_ifft_openmp.cpp",
            "cooley-tukey-inverse-fft-openmp.cpp",
            "../../../utils/bit_reversal.cpp",
            "-I../../..",
            "-fopenmp",
            "-o", "ifft_openmp_exec",
            "-std=c++17",
            "-lm"
        ]
    )
    assert compile_result.returncode == 0, "Compilazione IFFT OpenMP fallita"

    run_result = subprocess.run(["./ifft_openmp_exec"])
    assert run_result.returncode == 0, "Esecuzione IFFT OpenMP fallita"

    cpp_output = read_complex_output("output.txt")
    numpy_output = np.fft.ifft(freq_data)

    np.testing.assert_allclose(cpp_output, numpy_output, rtol=1e-5, atol=1e-8)

    os.remove("input.txt")
    os.remove("output.txt")
    os.remove("ifft_openmp_exec")
