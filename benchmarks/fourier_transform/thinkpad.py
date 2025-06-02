from plot_performance import *


_specs = "(Thinkpad, i7-3632QM CPU @ 2.20GHz)"


def _one_d_benchmark(files):
    df_seq = load_benchmark(files["1D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["1D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["1D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["1D_OpenMP_8"], "OpenMP 8 threads")

#    plot_execution_time(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "1D FFT Execution Time (All threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_execution_time_all_threads.pdf",
#            "./thinkpad/results/1D_fft_execution_time_all_threads.png"
#        ]
#    )
#    plot_multi_thread_speedup(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "Sequential",
#        "1D FFT Speedup (All threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_speedup_all_threads.pdf",
#            "./thinkpad/results/1D_fft_speedup_all_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_2,
#        "1D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_speedup_bar_2_threads.pdf",
#            "./thinkpad/results/1D_fft_speedup_bar_2_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_4,
#        "1D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_speedup_bar_4_threads.pdf",
#            "./thinkpad/results/1D_fft_speedup_bar_4_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_8,
#        "1D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_speedup_bar_8_threads.pdf",
#            "./thinkpad/results/1D_fft_speedup_bar_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_2,
#        2,
#        "1D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_efficiency_bar_2_threads.pdf",
#            "./thinkpad/results/1D_fft_efficiency_bar_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_4,
#        4,
#        "1D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_efficiency_bar_4_threads.pdf",
#            "./thinkpad/results/1D_fft_efficiency_bar_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_8,
#        8,
#        "1D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./thinkpad/results/1D_fft_efficiency_bar_8_threads.pdf",
#            "./thinkpad/results/1D_fft_efficiency_bar_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"1D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./thinkpad/results/1D_fft_speedup_vs_threads.pdf",
            "./thinkpad/results/1D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"1D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./thinkpad/results/1D_fft_efficiency_vs_threads.pdf",
            "./thinkpad/results/1D_fft_efficiency_vs_threads.png"
        ]
    )


def _two_d_benchmark(files):
    df_seq = load_benchmark(files["2D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["2D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["2D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["2D_OpenMP_8"], "OpenMP 8 threads")

#    plot_execution_time_binned(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "2D FFT Execution Time (All threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_execution_time_binned_all_threads.pdf",
#            "./thinkpad/results/2D_fft_execution_time_binned_all_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_2,
#        f"2D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_speedup_binned_with_counts_2_threads.pdf",
#            "./thinkpad/results/2D_fft_speedup_binned_with_counts_2_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_4,
#        f"2D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_speedup_binned_with_counts_4_threads.pdf",
#            "./thinkpad/results/2D_fft_speedup_binned_with_counts_4_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_8,
#        f"2D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_speedup_binned_with_counts_8_threads.pdf",
#            "./thinkpad/results/2D_fft_speedup_binned_with_counts_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_2,
#        2,
#        "2D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_efficiency_bar_binned_2_threads.pdf",
#            "./thinkpad/results/2D_fft_efficiency_bar_binned_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_4,
#        4,
#        "2D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_efficiency_bar_binned_4_threads.pdf",
#            "./thinkpad/results/2D_fft_efficiency_bar_binned_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_8,
#        8,
#        "2D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./thinkpad/results/2D_fft_efficiency_bar_binned_8_threads.pdf",
#            "./thinkpad/results/2D_fft_efficiency_bar_binned_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"2D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./thinkpad/results/2D_fft_speedup_vs_threads.pdf",
            "./thinkpad/results/2D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"2D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./thinkpad/results/2D_fft_efficiency_vs_threads.pdf",
            "./thinkpad/results/2D_fft_efficiency_vs_threads.png"
        ]
    )


def _three_d_benchmark(files):
    df_seq = load_benchmark(files["3D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["3D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["3D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["3D_OpenMP_8"], "OpenMP 8 threads")

#    plot_execution_time(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "3D FFT Execution Time (All threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_execution_time_all_threads.pdf",
#            "./thinkpad/results/3D_fft_execution_time_all_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_2,
#        f"3D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_speedup_binned_with_counts_2_threads.pdf",
#            "./thinkpad/results/3D_fft_speedup_binned_with_counts_2_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_4,
#        f"3D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_speedup_binned_with_counts_4_threads.pdf",
#            "./thinkpad/results/3D_fft_speedup_binned_with_counts_4_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_8,
#        f"3D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_speedup_binned_with_counts_8_threads.pdf",
#            "./thinkpad/results/3D_fft_speedup_binned_with_counts_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_2,
#        2,
#        "3D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_efficiency_bar_binned_2_threads.pdf",
#            "./thinkpad/results/3D_fft_efficiency_bar_binned_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_4,
#        4,
#        "3D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_efficiency_bar_binned_4_threads.pdf",
#            "./thinkpad/results/3D_fft_efficiency_bar_binned_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_8,
#        8,
#        "3D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./thinkpad/results/3D_fft_efficiency_bar_binned_8_threads.pdf",
#            "./thinkpad/results/3D_fft_efficiency_bar_binned_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"3D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./thinkpad/results/3D_fft_speedup_vs_threads.pdf",
            "./thinkpad/results/3D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"3D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./thinkpad/results/3D_fft_efficiency_vs_threads.pdf",
            "./thinkpad/results/3D_fft_efficiency_vs_threads.png"
        ]
    )



def main():
    files = {
        "1D_Sequential": "./thinkpad/thinkpad-1D_results_sequential_2025-05-19_12-07-52.json",
        "1D_OpenMP_8": "./thinkpad/thinkpad-1D_results_openmp_threads_8_2025-05-18_17-13-24.json",
        "1D_OpenMP_4": "./thinkpad/thinkpad-1D_results_openmp_threads_4_2025-05-25_15-33-57.json",
        "1D_OpenMP_2": "./thinkpad/thinkpad-1D_results_openmp_threads_2_2025-05-25_15-37-47.json",

        "2D_Sequential": "./thinkpad/thinkpad-2D_results_sequential_2025-05-26_10-21-59.json",
        "2D_OpenMP_8": "./thinkpad/thinkpad-2D_results_openmp_threads_8_2025-05-19_12-41-02.json",
        "2D_OpenMP_4": "./thinkpad/thinkpad-2D_results_openmp_threads_4_2025-05-25_15-13-23.json",
        "2D_OpenMP_2": "./thinkpad/thinkpad-2D_results_openmp_threads_2_2025-05-25_15-19-40.json",

        "3D_Sequential": "./thinkpad/thinkpad-3D_results_sequential_2025-05-18_16-06-50.json",
        "3D_OpenMP_8": "./thinkpad/thinkpad-3D_results_openmp_threads_8_2025-05-19_22-47-15.json",
        "3D_OpenMP_4": "./thinkpad/thinkpad-3D_results_openmp_threads_4_2025-05-25_13-26-36.json",
        "3D_OpenMP_2": "./thinkpad/thinkpad-3D_results_openmp_threads_2_2025-05-25_14-18-53.json"
    }

    ### 1D FFT Performance Analysis
    _one_d_benchmark(files)

    #### 2D FFT Performance Analysis
    _two_d_benchmark(files)

    #### 3D FFT Performance Analysis
    _three_d_benchmark(files)