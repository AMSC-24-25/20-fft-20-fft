import subprocess
import os
import numpy as np

def test_haar_wavelet_2d():
    subprocess.run([
        "g++",
        "Demo.cpp",
        "-o", "haar_demo",
        "-std=c++17",
        "-lm"
    ], check=True)

    result = subprocess.run(["./haar_demo"], capture_output=True, text=True)
    output = result.stdout

    # Verifica che il programma stampi la matrice Hn e la soluzione
    assert "Corresponding Hn matrix:" in output
    assert "Solution matrix:" in output

    # Cleanup
    os.remove("haar_demo")
