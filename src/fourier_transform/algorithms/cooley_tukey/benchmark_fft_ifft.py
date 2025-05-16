import numpy as np
import subprocess
import time
import matplotlib.pyplot as plt
import csv

def generate_input_file(N):
    """Genera file 'input.txt' con N numeri complessi casuali."""
    data = np.random.rand(N) + 1j * np.random.rand(N)
    with open("input.txt", "w") as f:
        f.write(f"{N}\n")
        for c in data:
            f.write(f"{c.real} {c.imag}\n")

def measure_time(executable, N):
    generate_input_file(N)
    start = time.perf_counter()
    subprocess.run([f"./{executable}"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    end = time.perf_counter()
    return end - start

def benchmark(name, executable, sizes):
    times = []
    for N in sizes:
        print(f"▶️  Eseguendo {name} con N = {N}")
        t = measure_time(executable, N)
        times.append(t)
    return times

def main():
    sizes = [2**i for i in range(6, 14)]  # da 64 a 8192
    results = {
        "FFT sequenziale": benchmark("FFT seq", "fft_seq_exec", sizes),
        "FFT OpenMP": benchmark("FFT omp", "fft_omp_exec", sizes),
        "IFFT sequenziale": benchmark("IFFT seq", "ifft_seq_exec", sizes),
        "IFFT OpenMP": benchmark("IFFT omp", "ifft_omp_exec", sizes),
    }

    # ✅ Salva risultati CSV
    with open("benchmark_results.csv", "w", newline="") as f:
        writer = csv.writer(f)
        header = ["N"] + list(results.keys())
        writer.writerow(header)
        for i, N in enumerate(sizes):
            row = [N] + [results[k][i] for k in results]
            writer.writerow(row)

    # ✅ Plot con etichette e salvataggio PNG
    plt.figure(figsize=(10, 6))
    for label, times in results.items():
        plt.plot(sizes, times, marker="o", label=label)
        for x, y in zip(sizes, times):
            plt.text(x, y, f"{y:.3f}s", fontsize=8, ha='left', va='bottom')

    plt.xlabel("Dimensione input N")
    plt.ylabel("Tempo di esecuzione (s)")
    plt.title("Performance FFT/IFFT - Sequenziale vs OpenMP")
    plt.xscale("log", base=2)
    plt.yscale("log")
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig("benchmark_fft_ifft.png", dpi=300)
    plt.show()

if __name__ == "__main__":
    main()
