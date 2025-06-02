from plot_performance import *


_specs = "(HP Pavilion 15-cx0xxx, i7-8750H CPU @ 2.20GHz)"

def _one_d_benchmark(files):
    df_seq = load_benchmark(files["1D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["1D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["1D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["1D_OpenMP_8"], "OpenMP 8 threads")
    df_omp_12 = load_benchmark(files["1D_OpenMP_12"], "OpenMP 12 threads")

#    plot_execution_time(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "1D FFT Execution Time (All threads)",
#        output_files=[
#            "./hp/results/1D_fft_execution_time_all_threads.pdf",
#            "./hp/results/1D_fft_execution_time_all_threads.png"
#        ]
#    )
#    plot_multi_thread_speedup(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "Sequential",
#        "1D FFT Speedup (All threads)",
#        output_files=[
#            "./hp/results/1D_fft_speedup_all_threads.pdf",
#            "./hp/results/1D_fft_speedup_all_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_2,
#        "1D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./hp/results/1D_fft_speedup_bar_2_threads.pdf",
#            "./hp/results/1D_fft_speedup_bar_2_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_4,
#        "1D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./hp/results/1D_fft_speedup_bar_4_threads.pdf",
#            "./hp/results/1D_fft_speedup_bar_4_threads.png"
#        ]
#    )
#    plot_speedup_bar(
#        df_seq,
#        df_omp_8,
#        "1D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./hp/results/1D_fft_speedup_bar_8_threads.pdf",
#            "./hp/results/1D_fft_speedup_bar_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_2,
#        2,
#        "1D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./hp/results/1D_fft_efficiency_bar_2_threads.pdf",
#            "./hp/results/1D_fft_efficiency_bar_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_4,
#        4,
#        "1D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./hp/results/1D_fft_efficiency_bar_4_threads.pdf",
#            "./hp/results/1D_fft_efficiency_bar_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar(
#        df_seq,
#        df_omp_8,
#        8,
#        "1D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./hp/results/1D_fft_efficiency_bar_8_threads.pdf",
#            "./hp/results/1D_fft_efficiency_bar_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
        12: df_omp_12
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"1D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./hp/results/1D_fft_speedup_vs_threads.pdf",
            "./hp/results/1D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"1D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./hp/results/1D_fft_efficiency_vs_threads.pdf",
            "./hp/results/1D_fft_efficiency_vs_threads.png"
        ]
    )


def _two_d_benchmark(files):
    df_seq = load_benchmark(files["2D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["2D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["2D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["2D_OpenMP_8"], "OpenMP 8 threads")
    df_omp_12 = load_benchmark(files["2D_OpenMP_12"], "OpenMP 12 threads")

#    plot_execution_time_binned(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "2D FFT Execution Time (All threads)",
#        output_files=[
#            "./hp/results/2D_fft_execution_time_binned_all_threads.pdf",
#            "./hp/results/2D_fft_execution_time_binned_all_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_2,
#        f"2D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./hp/results/2D_fft_speedup_binned_with_counts_2_threads.pdf",
#            "./hp/results/2D_fft_speedup_binned_with_counts_2_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_4,
#        f"2D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./hp/results/2D_fft_speedup_binned_with_counts_4_threads.pdf",
#            "./hp/results/2D_fft_speedup_binned_with_counts_4_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_8,
#        f"2D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./hp/results/2D_fft_speedup_binned_with_counts_8_threads.pdf",
#            "./hp/results/2D_fft_speedup_binned_with_counts_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_2,
#        2,
#        "2D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./hp/results/2D_fft_efficiency_bar_binned_2_threads.pdf",
#            "./hp/results/2D_fft_efficiency_bar_binned_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_4,
#        4,
#        "2D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./hp/results/2D_fft_efficiency_bar_binned_4_threads.pdf",
#            "./hp/results/2D_fft_efficiency_bar_binned_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_8,
#        8,
#        "2D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./hp/results/2D_fft_efficiency_bar_binned_8_threads.pdf",
#            "./hp/results/2D_fft_efficiency_bar_binned_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
        12: df_omp_12
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"2D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./hp/results/2D_fft_speedup_vs_threads.pdf",
            "./hp/results/2D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"2D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./hp/results/2D_fft_efficiency_vs_threads.pdf",
            "./hp/results/2D_fft_efficiency_vs_threads.png"
        ]
    )


def _three_d_benchmark(files):
    df_seq = load_benchmark(files["3D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["3D_OpenMP_2"], "OpenMP 2 threads")
    df_omp_4 = load_benchmark(files["3D_OpenMP_4"], "OpenMP 4 threads")
    df_omp_8 = load_benchmark(files["3D_OpenMP_8"], "OpenMP 8 threads")
    df_omp_12 = load_benchmark(files["3D_OpenMP_12"], "OpenMP 12 threads")

#    plot_execution_time(
#        pd.concat([df_seq, df_omp_2, df_omp_4, df_omp_8]),
#        "3D FFT Execution Time (All threads)",
#        output_files=[
#            "./hp/results/3D_fft_execution_time_all_threads.pdf",
#            "./hp/results/3D_fft_execution_time_all_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_2,
#        f"3D FFT Speedup Bar Chart (2 threads)",
#        output_files=[
#            "./hp/results/3D_fft_speedup_binned_with_counts_2_threads.pdf",
#            "./hp/results/3D_fft_speedup_binned_with_counts_2_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_4,
#        f"3D FFT Speedup Bar Chart (4 threads)",
#        output_files=[
#            "./hp/results/3D_fft_speedup_binned_with_counts_4_threads.pdf",
#            "./hp/results/3D_fft_speedup_binned_with_counts_4_threads.png"
#        ]
#    )
#    plot_speedup_binned_with_counts(
#        df_seq,
#        df_omp_8,
#        f"3D FFT Speedup Bar Chart (8 threads)",
#        output_files=[
#            "./hp/results/3D_fft_speedup_binned_with_counts_8_threads.pdf",
#            "./hp/results/3D_fft_speedup_binned_with_counts_8_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_2,
#        2,
#        "3D FFT Efficiency Bar Chart (2 threads)",
#        output_files=[
#            "./hp/results/3D_fft_efficiency_bar_binned_2_threads.pdf",
#            "./hp/results/3D_fft_efficiency_bar_binned_2_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_4,
#        4,
#        "3D FFT Efficiency Bar Chart (4 threads)",
#        output_files=[
#            "./hp/results/3D_fft_efficiency_bar_binned_4_threads.pdf",
#            "./hp/results/3D_fft_efficiency_bar_binned_4_threads.png"
#        ]
#    )
#    plot_efficiency_bar_binned(
#        df_seq,
#        df_omp_8,
#        8,
#        "3D FFT Efficiency Bar Chart (8 threads)",
#        output_files=[
#            "./hp/results/3D_fft_efficiency_bar_binned_8_threads.pdf",
#            "./hp/results/3D_fft_efficiency_bar_binned_8_threads.png"
#        ]
#    )
    df_omp_dict = {
        2: df_omp_2,
        4: df_omp_4,
        8: df_omp_8,
        12: df_omp_12
    }
    plot_speedup_vs_threads(
        df_seq,
        df_omp_dict,
        f"3D FFT Speedup vs Threads {_specs}",
        output_files=[
            "./hp/results/3D_fft_speedup_vs_threads.pdf",
            "./hp/results/3D_fft_speedup_vs_threads.png"
        ]
    )
    plot_efficiency_vs_threads(
        df_seq,
        df_omp_dict,
        f"3D FFT Efficiency vs Threads {_specs}",
        output_files=[
            "./hp/results/3D_fft_efficiency_vs_threads.pdf",
            "./hp/results/3D_fft_efficiency_vs_threads.png"
        ]
    )



def main():
    files = {
        "1D_Sequential": "./hp/hp-1D_results_sequential_2025-05-18_13-18-05.json",
        "1D_OpenMP_12": "./hp/hp-1D_results_openmp_threads_12_2025-05-20_17-30-27.json",
        "1D_OpenMP_8": "./hp/hp-1D_results_openmp_threads_8_2025-05-25_12-14-36.json",
        "1D_OpenMP_4": "./hp/hp-1D_results_openmp_threads_4_2025-05-25_12-51-23.json",
        "1D_OpenMP_2": "./hp/hp-1D_results_openmp_threads_2_2025-05-25_12-57-05.json",

        "2D_Sequential": "./hp/hp-2D_results_sequential_2025-05-18_13-19-45.json",
        "2D_OpenMP_12": "./hp/hp-2D_results_openmp_threads_12_2025-05-20_17-33-41.json",
        "2D_OpenMP_8": "./hp/hp-2D_results_openmp_threads_8_2025-05-25_12-58-57.json",
        "2D_OpenMP_4": "./hp/hp-2D_results_openmp_threads_4_2025-05-25_13-08-24.json",
        "2D_OpenMP_2": "./hp/hp-2D_results_openmp_threads_2_2025-05-25_13-14-52.json",

        "3D_Sequential": "./hp/hp-3D_results_sequential_2025-05-18_16-06-44.json",
        "3D_OpenMP_12": "./hp/hp-3D_results_openmp_threads_12_2025-05-20_17-38-20.json",
        "3D_OpenMP_8": "./hp/hp-3D_results_openmp_threads_8_2025-05-25_13-21-06.json",
        "3D_OpenMP_4": "./hp/hp-3D_results_openmp_threads_4_2025-05-25_14-19-48.json",
        "3D_OpenMP_2": "./hp/hp-3D_results_openmp_threads_2_2025-05-25_15-01-28.json"
    }

    ### 1D FFT Performance Analysis
    _one_d_benchmark(files)

    #### 2D FFT Performance Analysis
    _two_d_benchmark(files)

    #### 3D FFT Performance Analysis
    _three_d_benchmark(files)