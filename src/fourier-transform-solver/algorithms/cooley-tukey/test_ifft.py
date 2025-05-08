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

def test_ifft_against_numpy():
    # 1. Genera dati nel dominio della frequenza
    N = 8
    np.random.seed(123)
    freq_data = np.random.rand(N) + 1j * np.random.rand(N)

    # 2. Scrivi input per il C++
    write_complex_input("input.txt", freq_data)

    # 3. Compila ed esegui il C++
    compile_result = subprocess.run(
        [
            "g++",
            "main_ifft.cpp",
            "cooley-tukey-inverse-fft.cpp",
            "../../../utils/bit-reversal.cpp",
            "-I../../..",
            "-o", "ifft_exec",
            "-std=c++17",
            "-lm"
        ]
    )
    assert compile_result.returncode == 0, "Compilazione IFFT fallita"

    run_result = subprocess.run(["./ifft_exec"])
    assert run_result.returncode == 0, "Esecuzione IFFT fallita"

    # 4. Confronta con NumPy
    cpp_output = read_complex_output("output.txt")
    numpy_output = np.fft.ifft(freq_data)

    np.testing.assert_allclose(cpp_output, numpy_output, rtol=1e-5, atol=1e-8)

    # Cleanup
    os.remove("input.txt")
    os.remove("output.txt")
    os.remove("ifft_exec")
