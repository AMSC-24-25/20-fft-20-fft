import json
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns


def load_benchmark(file_path, label):
    """
    Load benchmark data from a JSON file and convert it to a DataFrame.
    :param file_path: Path to the JSON file containing benchmark results.
    :param label: Label for the benchmark data (e.g., "Sequential", "OpenMP").
    :return: DataFrame containing the benchmark results with columns for size, time in milliseconds, and label.
    """
    with open(file_path, 'r') as f:
        raw = json.load(f)
    data = []
    for b in raw["benchmarks"]:
        name = b["name"]
        size_str: list[str] = name.split("/")[1].split("x")
        # size_str is a list of strings like ['1024', '1024', '']
        size = 1
        for s in size_str:
            if s.isdigit():
                size *= int(s)
            else:
                # if the string is not a digit, we skip it
                continue
        # convert time from nanoseconds to milliseconds
        time_ms = b["real_time"] / 1e6
        data.append({"size": size, "time_ms": time_ms, "label": label})
    return pd.DataFrame(data)


def plot_execution_time(df, title, output_files: list | None = None):
    """
    Plot execution time for different benchmarks.
    The DataFrame should contain columns for size, time_ms, and label.
    :param df: DataFrame containing benchmark results with columns 'size', 'time_ms', and 'label'.
    :param title: Title for the plot.
    :param output_file: Optional; if provided, the plot will be saved to this file.
    """
    plt.figure(figsize=(10, 5))
    for label, group in df.groupby("label"):
        group_sorted = group.sort_values("size")
        plt.plot(group_sorted["size"], group_sorted["time_ms"], marker='o', label=label)
    plt.xlabel("Input Size")
    plt.ylabel("Execution Time (ms)")
    plt.title(title)
    plt.grid(True)
    plt.legend()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_speedup(df_seq, df_omp, title, output_files: list | None = None):
    """
    Plot speedup of OpenMP over Sequential implementation.
    The DataFrame should contain columns for size, time_ms_seq, and time_ms_omp.
    :param df_seq: DataFrame containing sequential benchmark results with columns 'size' and 'time_ms'.
    :param df_omp: DataFrame containing OpenMP benchmark results with columns 'size' and 'time_ms'.
    :param title: Title for the plot.
    :param output_file: Optional; if provided, the plot will be saved to this file.
    """
    merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", "_omp"))
    merged["speedup"] = merged["time_ms_seq"] / merged["time_ms_omp"]

    plt.figure(figsize=(10, 5))
    plt.plot(
        merged["size"], merged["speedup"],
        marker='o', markersize=8, linewidth=2,
        markerfacecolor='white', markeredgecolor='green',
        color='green', label="Speedup"
    )
    plt.axhline(1.0, color='red', linestyle='--', linewidth=1)

    # Highlight max and min points with text
    max_row = merged.loc[merged["speedup"].idxmax()]
    min_row = merged.loc[merged["speedup"].idxmin()]
    for row in [max_row, min_row]:
        plt.annotate(
            f'{row["speedup"]:.2f}',
            xy=(row["size"], row["speedup"]),
            xytext=(0, 10),
            textcoords='offset points',
            ha='center',
            fontsize=9,
            color='black',
            bbox=dict(boxstyle="round,pad=0.2", fc="yellow", alpha=0.3)
        )

    plt.xlabel("Input Size")
    plt.ylabel("Speedup (Sequential / OpenMP)")
    plt.title(title)
    plt.grid(True)
    plt.legend()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_speedup_bar(df_seq, df_omp, title, output_files: list | None = None):
    sns.set(style="whitegrid")

    merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", "_omp"))
    merged["speedup"] = merged["time_ms_seq"] / merged["time_ms_omp"]
    merged = merged.sort_values("size")

    # Color mapping based on speedup value
    def color_map(s):
        if s < 1.0:
            return 'red'
        elif s < 1.5:
            return 'orange'
        else:
            return 'green'
    colors = merged["speedup"].apply(color_map)

    plt.figure(figsize=(12, 6))
    bars = plt.bar(merged["size"].astype(str), merged["speedup"], color=colors)

    # Annotate bars with speedup value
    for bar, val in zip(bars, merged["speedup"]):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height + 0.05,
                 f'{val:.2f}', ha='center', va='bottom', fontsize=8)

    plt.axhline(1.0, color='black', linestyle='--', linewidth=1)
    plt.xlabel("Input Size")
    plt.ylabel("Speedup (Sequential / OpenMP)")
    plt.title(title)
    # plt.xticks(rotation=45, ha='right')
    step = max(1, len(merged) // 20)
    plt.xticks(
        ticks=range(0, len(merged), step),
        labels=merged["size"].astype(str).values[::step],
        rotation=45,
        ha='right',
        fontsize=8
    )
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_speedup_binned(df_seq, df_omp, title, output_files: list | None = None):
    # Merge and compute speedup
    merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", "_omp"))
    merged["speedup"] = merged["time_ms_seq"] / merged["time_ms_omp"]
    merged["log2_size"] = np.log2(merged["size"]).astype(int)

    # Group by log2 bins
    binned = merged.groupby("log2_size").agg(
        avg_speedup=("speedup", "mean"),
        max_speedup=("speedup", "max"),
        min_speedup=("speedup", "min"),
        count=("speedup", "count")
    ).reset_index()

    # Plot average speedup bar
    plt.figure(figsize=(10, 6))
    plt.bar(binned["log2_size"], binned["avg_speedup"], color='steelblue', alpha=0.8)
    plt.axhline(1.0, color='red', linestyle='--')
    plt.xlabel("log2(Input Size)")
    plt.ylabel("Average Speedup")
    plt.title(title)
    plt.grid(True)
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_speedup_binned_with_counts(df_seq, df_omp, title, output_files: list | None = None):
    # Merge and compute speedup
    merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", "_omp"))
    merged["speedup"] = merged["time_ms_seq"] / merged["time_ms_omp"]
    merged["log2_size"] = np.log2(merged["size"]).astype(int)

    # Group by log2 bins
    binned = merged.groupby("log2_size").agg(
        avg_speedup=("speedup", "mean"),
        max_speedup=("speedup", "max"),
        std_speedup=("speedup", "std"),
        count=("speedup", "count")
    ).reset_index()

    # Plot dual bars (avg + max speedup)
    x = binned["log2_size"]
    width = 0.4

    fig, ax1 = plt.subplots(figsize=(12, 6))

    ax1.errorbar(
        binned["log2_size"],
        binned["avg_speedup"],
        yerr=binned["std_speedup"],
        fmt='none',
        ecolor='black',
        elinewidth=1.2,
        capsize=4,
        label="Std Dev"
    )
    ax1.bar(x - width/2, binned["avg_speedup"], width=width, label='Avg Speedup', color='steelblue')
    ax1.bar(x + width/2, binned["max_speedup"], width=width, label='Max Speedup', color='orange')
    ax1.axhline(1.0, color='red', linestyle='--')
    ax1.set_xlabel("log2(Input Size)")
    ax1.set_ylabel("Speedup (Sequential / OpenMP)")
    ax1.set_title(title)
    ax1.grid(True)

    # Add sample count on secondary axis
    ax2 = ax1.twinx()
    ax2.plot(x, binned["count"], color='gray', marker='o', linestyle='-', label='Sample Count')
    ax2.set_ylabel("Number of Samples per Bin", color='gray')
    ax2.tick_params(axis='y', labelcolor='gray')

    # Combine legends from both axes
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_2, labels_2 = ax2.get_legend_handles_labels()
    ax1.legend(lines_1 + lines_2, labels_1 + labels_2, loc='upper left')

    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_efficiency_bar(df_seq, df_omp, num_threads, title, output_files: list | None = None):
    merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", "_omp"))
    merged["speedup"] = merged["time_ms_seq"] / merged["time_ms_omp"]
    merged["efficiency"] = merged["speedup"] / num_threads

    plt.figure(figsize=(10, 6))
    bars = plt.bar(merged["size"].astype(str), merged["efficiency"], color='skyblue')

    # Annotate each bar
    for bar, val in zip(bars, merged["efficiency"]):
        plt.text(bar.get_x() + bar.get_width()/2, val + 0.01, f'{val:.2f}', ha='center', va='bottom', fontsize=8)

    plt.axhline(1.0, linestyle="--", color="red", label="100% Efficiency")
    plt.xlabel("Input Size")
    plt.ylabel("Efficiency (Speedup / Threads)")
    plt.title(title)
    plt.ylim(0, 1.0)
    plt.xticks(rotation=45, ha='right')
    plt.grid(True)
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()



if __name__ == "__main__":
    specs = "Thinkpad, 8 threads, 1.20 GHz - 3.20 GHz"
    files = {
        "1D_Sequential": "./thinkpad/1D_results_sequential_2025-05-19_12-07-52.json",
        "1D_OpenMP": "./thinkpad/1D_results_openmp_2025-05-18_17-13-24.json",
        "2D_Sequential": "./thinkpad/2D_results_sequential_2025-05-19_12-08-23.json",
        "2D_OpenMP": "./thinkpad/2D_results_openmp_2025-05-19_12-41-02.json",
        "3D_Sequential": "./thinkpad/3D_results_sequential_2025-05-18_16-06-50.json",
        "3D_OpenMP": "./thinkpad/3D_results_openmp_2025-05-19_22-47-15.json"
    }

    ### 1D FFT Performance Analysis

    df_seq = load_benchmark(files["1D_Sequential"], "Sequential")
    df_omp = load_benchmark(files["1D_OpenMP"], "OpenMP")
    df_all = pd.concat([df_seq, df_omp])

    plot_execution_time(
        df_all,
        f"1D FFT Execution Time ({specs})",
        output_files=[
            "./thinkpad/1D_fft_execution_time.pdf",
            "./thinkpad/1D_fft_execution_time.png"
        ]
    )
    plot_speedup_bar(
        df_seq,
        df_omp,
        f"1D FFT Speedup Bar Chart ({specs})",
        output_files=[
            "./thinkpad/1D_fft_speedup_bar.pdf",
            "./thinkpad/1D_fft_speedup_bar.png"
        ]
    )
    plot_efficiency_bar(
        df_seq,
        df_omp,
        num_threads=8,
        title=f"1D FFT Parallel Efficiency with 8 threads ({specs})",
        output_files=[
            "./thinkpad/1D_fft_parallel_efficiency.pdf",
            "./thinkpad/1D_fft_parallel_efficiency.png"
        ]
    )

    #### 2D FFT Performance Analysis

    df_seq_2 = load_benchmark(files["2D_Sequential"], "Sequential")
    df_omp_2 = load_benchmark(files["2D_OpenMP"], "OpenMP")
    df_all_2 = pd.concat([df_seq_2, df_omp_2])

    plot_execution_time(
        df_all_2,
        f"2D FFT Execution Time ({specs})",
        output_files=[
            "./thinkpad/2D_fft_execution_time.pdf",
            "./thinkpad/2D_fft_execution_time.png"
        ]
    )
    plot_speedup_binned_with_counts(
        df_seq_2,
        df_omp_2,
        f"2D FFT Speedup Bar Chart ({specs})",
        output_files=[
            "./thinkpad/2D_fft_speedup_binned_with_counts.pdf",
            "./thinkpad/2D_fft_speedup_binned_with_counts.png"
        ]
    )

    #### 3D FFT Performance Analysis

    df_seq_3 = load_benchmark(files["3D_Sequential"], "Sequential")
    df_omp_3 = load_benchmark(files["3D_OpenMP"], "OpenMP")
    df_all_3 = pd.concat([df_seq_3, df_omp_3])

    plot_execution_time(
        df_all_3,
        f"3D FFT Execution Time ({specs})",
        output_files=[
            "./thinkpad/3D_fft_execution_time.pdf",
            "./thinkpad/3D_fft_execution_time.png"
        ]
    )
    plot_speedup_binned_with_counts(
        df_seq_3,
        df_omp_3,
        f"3D FFT Speedup Binned ({specs})",
        output_files=[
            "./thinkpad/3D_fft_speedup_binned_with_counts.pdf",
            "./thinkpad/3D_fft_speedup_binned_with_counts.png"
        ]
    )
