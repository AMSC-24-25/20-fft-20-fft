from plot_performance import *


_specs = "(Dell Inspiron 14 Plus, Intel Ultra 7 155H, 700 MHz to 4.8 GHz)"


def _one_d_benchmark(files):
    df_seq = load_benchmark(files["1D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["1D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["1D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["1D_OpenMP_8"], "OpenMP 8 threads")
    df_omp_16 = load_benchmark(files["1D_OpenMP_16"], "OpenMP 16 threads")
    df_omp_22 = load_benchmark(files["1D_OpenMP_22"], "OpenMP 22 threads")

#    plot_execution_time(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "1D FFT Execution Time (All threads)",
#        output_files=[
#            "./dell/results/1D_fft_execution_time_all_threads.pdf",
#            "./dell/results/1D_fft_execution_time_all_threads.png"
#        ]
#    )
#    plot_multi_thread_speedup(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "Sequential",
#        "1D FFT Speedup (All threads)",
#        output_files=[
#            "./dell/results/1D_fft_speedup_all_threads.pdf",
#            "./dell/results/1D_fft_speedup_all_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_2,
#        "1D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./dell/results/1D_fft_speedup_bar_2_threads.pdf",
#            "./dell/results/1D_fft_speedup_bar_2_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_4,
#        "1D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./dell/results/1D_fft_speedup_bar_4_threads.pdf",
#            "./dell/results/1D_fft_speedup_bar_4_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_8,
#        "1D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./dell/results/1D_fft_speedup_bar_8_threads.pdf",
#            "./dell/results/1D_fft_speedup_bar_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_2,
#        2,
#        "1D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./dell/results/1D_fft_efficiency_bar_2_threads.pdf",
#            "./dell/results/1D_fft_efficiency_bar_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_4,
#        4,
#        "1D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./dell/results/1D_fft_efficiency_bar_4_threads.pdf",
#            "./dell/results/1D_fft_efficiency_bar_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_8,
#        8,
#        "1D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./dell/results/1D_fft_efficiency_bar_8_threads.pdf",
#            "./dell/results/1D_fft_efficiency_bar_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
        16: df_omp_16,
        22: df_omp_22
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"1D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./dell/results/1D_fft_speedup_vs_threads.pdf",
            "./dell/results/1D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"1D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./dell/results/1D_fft_efficiency_vs_threads.pdf",
            "./dell/results/1D_fft_efficiency_vs_threads.png"
        ]
    )


def _two_d_benchmark(files):
    df_seq = load_benchmark(files["2D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["2D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["2D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["2D_OpenMP_8"], "OpenMP 8 threads")
    df_omp_16 = load_benchmark(files["2D_OpenMP_16"], "OpenMP 16 threads")
    df_omp_22 = load_benchmark(files["2D_OpenMP_22"], "OpenMP 22 threads")

#    plot_execution_time_binned(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "2D FFT Execution Time (All threads)",
#        output_files=[
#            "./dell/results/2D_fft_execution_time_binned_all_threads.pdf",
#            "./dell/results/2D_fft_execution_time_binned_all_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_2,
#        f"2D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./dell/results/2D_fft_speedup_binned_with_counts_2_threads.pdf",
#            "./dell/results/2D_fft_speedup_binned_with_counts_2_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_4,
#        f"2D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./dell/results/2D_fft_speedup_binned_with_counts_4_threads.pdf",
#            "./dell/results/2D_fft_speedup_binned_with_counts_4_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_8,
#        f"2D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./dell/results/2D_fft_speedup_binned_with_counts_8_threads.pdf",
#            "./dell/results/2D_fft_speedup_binned_with_counts_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_2,
#        2,
#        "2D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./dell/results/2D_fft_efficiency_bar_binned_2_threads.pdf",
#            "./dell/results/2D_fft_efficiency_bar_binned_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_4,
#        4,
#        "2D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./dell/results/2D_fft_efficiency_bar_binned_4_threads.pdf",
#            "./dell/results/2D_fft_efficiency_bar_binned_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_8,
#        8,
#        "2D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./dell/results/2D_fft_efficiency_bar_binned_8_threads.pdf",
#            "./dell/results/2D_fft_efficiency_bar_binned_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
        16: df_omp_16,
        22: df_omp_22
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"2D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./dell/results/2D_fft_speedup_vs_threads.pdf",
            "./dell/results/2D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"2D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./dell/results/2D_fft_efficiency_vs_threads.pdf",
            "./dell/results/2D_fft_efficiency_vs_threads.png"
        ]
    )


def _three_d_benchmark(files):
    df_seq = load_benchmark(files["3D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["3D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["3D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["3D_OpenMP_8"], "OpenMP 8 threads")
    df_omp_16 = load_benchmark(files["3D_OpenMP_16"], "OpenMP 16 threads")
    df_omp_22 = load_benchmark(files["3D_OpenMP_22"], "OpenMP 22 threads")

#    plot_execution_time(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "3D FFT Execution Time (All threads)",
#        output_files=[
#            "./dell/results/3D_fft_execution_time_all_threads.pdf",
#            "./dell/results/3D_fft_execution_time_all_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_2,
#        f"3D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./dell/results/3D_fft_speedup_binned_with_counts_2_threads.pdf",
#            "./dell/results/3D_fft_speedup_binned_with_counts_2_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_4,
#        f"3D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./dell/results/3D_fft_speedup_binned_with_counts_4_threads.pdf",
#            "./dell/results/3D_fft_speedup_binned_with_counts_4_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_8,
#        f"3D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./dell/results/3D_fft_speedup_binned_with_counts_8_threads.pdf",
#            "./dell/results/3D_fft_speedup_binned_with_counts_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_2,
#        2,
#        "3D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./dell/results/3D_fft_efficiency_bar_binned_2_threads.pdf",
#            "./dell/results/3D_fft_efficiency_bar_binned_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_4,
#        4,
#        "3D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./dell/results/3D_fft_efficiency_bar_binned_4_threads.pdf",
#            "./dell/results/3D_fft_efficiency_bar_binned_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_8,
#        8,
#        "3D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./dell/results/3D_fft_efficiency_bar_binned_8_threads.pdf",
#            "./dell/results/3D_fft_efficiency_bar_binned_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
        16: df_omp_16,
        22: df_omp_22
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"3D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./dell/results/3D_fft_speedup_vs_threads.pdf",
            "./dell/results/3D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"3D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./dell/results/3D_fft_efficiency_vs_threads.pdf",
            "./dell/results/3D_fft_efficiency_vs_threads.png"
        ]
    )



def main():
    files = {
        "1D_Sequential": "./dell/dell-1D_results_sequential_2025-05-25_10-19-35.json",
        "1D_OpenMP_22": "./dell/dell-1D_results_openmp_threads_22_2025-05-25_10-24-16.json",
        "1D_OpenMP_16": "./dell/dell-1D_results_openmp_threads_16_2025-05-25_12-57-14.json",
        "1D_OpenMP_8": "./dell/dell-1D_results_openmp_threads_8_2025-05-25_12-59-09.json",
        "1D_OpenMP_4": "./dell/dell-1D_results_openmp_threads_4_2025-05-25_13-01-19.json",
        "1D_OpenMP_2": "./dell/dell-1D_results_openmp_threads_2_2025-05-25_13-08-14.json",

        "2D_Sequential": "./dell/dell-2D_results_sequential_2025-05-25_10-25-58.json",
        "2D_OpenMP_22": "./dell/dell-2D_results_openmp_threads_22_2025-05-25_10-42-51.json",
        "2D_OpenMP_16": "./dell/dell-2D_results_openmp_threads_16_2025-05-25_13-15-11.json",
        "2D_OpenMP_8": "./dell/dell-2D_results_openmp_threads_8_2025-05-25_13-20-48.json",
        "2D_OpenMP_4": "./dell/dell-2D_results_openmp_threads_4_2025-05-25_13-51-23.json",
        "2D_OpenMP_2": "./dell/dell-2D_results_openmp_threads_2_2025-05-25_14-19-12.json",

        "3D_Sequential": "./dell/dell-3D_results_sequential_2025-05-25_11-02-19.json",
        "3D_OpenMP_22": "./dell/dell-3D_results_openmp_threads_22_2025-05-25_11-43-35.json",
        "3D_OpenMP_16": "./dell/dell-3D_results_openmp_threads_16_2025-05-25_14-31-26.json",
        "3D_OpenMP_8": "./dell/dell-3D_results_openmp_threads_8_2025-05-25_15-01-18.json",
        "3D_OpenMP_4": "./dell/dell-3D_results_openmp_threads_4_2025-05-25_15-37-30.json",
        "3D_OpenMP_2": "./dell/dell-3D_results_openmp_threads_2_2025-05-25_16-07-27.json"
    }

    ### 1D FFT Performance Analysis
    _one_d_benchmark(files)

    #### 2D FFT Performance Analysis
    _two_d_benchmark(files)

    #### 3D FFT Performance Analysis
    _three_d_benchmark(files)