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

def test_fft_against_numpy():
    # 1. Genera input complesso
    N = 8
    np.random.seed(42)
    input_data = np.random.rand(N) + 1j * np.random.rand(N)

    # 2. Scrive input su file
    write_complex_input("input.txt", input_data)

    # 3. Compila ed esegue il C++
    compile_result = subprocess.run(
    [
        "g++",
        "main_fft.cpp",
        "cooley-tukey-fft.cpp",
        "../../../utils/bit_reversal.cpp",  # 👈 percorso corretto
        "-I../../..",                       # 👈 include headers da src/
        "-o", "fft_exec",
        "-std=c++17",
        "-lm"
    ]
)


    assert compile_result.returncode == 0, "Compilazione fallita"

    run_result = subprocess.run(["./fft_exec"])
    assert run_result.returncode == 0, "Esecuzione fallita"

    # 4. Legge output C++ e confronta con numpy
    cpp_output = read_complex_output("output.txt")
    numpy_output = np.fft.fft(input_data)

    np.testing.assert_allclose(cpp_output, numpy_output, rtol=1e-5, atol=1e-8)

    # Pulizia opzionale
    os.remove("input.txt")
    os.remove("output.txt")
    os.remove("fft_exec")
