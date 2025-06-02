import subprocess
import os
import numpy as np

def test_haar_wavelet_1d():
    # Compila
    subprocess.run([
        "g++",
        "Demo.cpp",
        "-o", "haar_demo",
        "-std=c++17",
        "-lm"
    ], check=True)

    # Esegui e cattura output
    result = subprocess.run(["./haar_demo"], capture_output=True, text=True)
    output = result.stdout

    # Estrai soluzione dal log
    start = output.find("Solution vector:")
    values = output[start:].split("[")[1].split("]")[0].strip().split()
    solution = np.array([float(v) for v in values])

    # Verifica che la lunghezza sia corretta
    assert len(solution) == 8

    # Verifica proprietà base (energia conservata, esempio base)
    energy_input = sum([x**2 for x in [1,2,3,1,2,3,4,0]])
    energy_output = np.sum(solution**2)
    np.testing.assert_allclose(energy_input, energy_output, rtol=1e-5)

    # Cleanup
    os.remove("haar_demo")
