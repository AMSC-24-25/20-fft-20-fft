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
    :param output_files: Optional; if provided, the plot will be saved to this file.
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
    :param output_files: Optional; if provided, the plot will be saved to this file.
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


def plot_efficiency_bar_binned(df_seq, df_omp, num_threads, title, output_files: list | None = None):
    merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", "_omp"))
    merged["speedup"] = merged["time_ms_seq"] / merged["time_ms_omp"]
    merged["efficiency"] = merged["speedup"] / num_threads
    merged["log2_size"] = np.log2(merged["size"]).astype(int)

    # Aggregate by log2_size
    binned = merged.groupby("log2_size").agg(
        avg_efficiency=("efficiency", "mean"),
        std_efficiency=("efficiency", "std"),
        count=("efficiency", "count")
    ).reset_index()

    plt.figure(figsize=(10, 6))
    plt.bar(binned["log2_size"], binned["avg_efficiency"], yerr=binned["std_efficiency"],
            color='skyblue', capsize=4)
    plt.axhline(1.0, linestyle="--", color="red", label="100% Efficiency")
    plt.xlabel("log2(Input Size)")
    plt.ylabel("Average Efficiency (Speedup / Threads)")
    plt.title(title)
    plt.ylim(0, 1.0)
    plt.grid(True)
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_multi_thread_speedup(df_all, baseline_label="Sequential", title="Speedup Across Threads", output_files=None):
    """
    Plot speedup curves for multiple OpenMP thread configurations against a baseline (e.g., Sequential).

    :param df_all: DataFrame containing all runs, with columns 'size', 'time_ms', and 'label'.
    :param baseline_label: Label used for the sequential baseline.
    :param title: Plot title.
    :param output_files: Optional list of output filenames to save.
    """
    # Separate baseline and parallel versions
    df_base = df_all[df_all["label"] == baseline_label][["size", "time_ms"]].rename(columns={"time_ms": "time_ms_base"})

    plt.figure(figsize=(12, 6))
    for label, group in df_all[df_all["label"] != baseline_label].groupby("label"):
        merged = pd.merge(df_base, group, on="size")
        merged["speedup"] = merged["time_ms_base"] / merged["time_ms"]
        merged_sorted = merged.sort_values("size")
        plt.plot(merged_sorted["size"], merged_sorted["speedup"], marker='o', label=label)

    plt.axhline(1.0, color='gray', linestyle='--')
    plt.xlabel("Input Size")
    plt.ylabel("Speedup (vs Sequential)")
    plt.title(title)
    plt.grid(True)
    plt.legend(title="Threads")
    plt.tight_layout()

    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_speedup_vs_threads(df_seq, df_omp_dict, title, output_files: list | None = None):
    """
    Compare binned average speedup for different thread counts on a single plot, with error bars.
    """
    plt.figure(figsize=(12, 6))
    for threads, df_omp in sorted(df_omp_dict.items()):
        merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", f"_omp_{threads}"))
        merged["speedup"] = merged["time_ms_seq"] / merged[f"time_ms_omp_{threads}"]
        merged["log2_size"] = np.log2(merged["size"]).astype(int)
        binned = merged.groupby("log2_size").agg(
            avg_speedup=("speedup", "mean"),
            std_speedup=("speedup", "std"),
            count=("speedup", "count")
        ).reset_index()
        plt.errorbar(
            binned["log2_size"], binned["avg_speedup"], yerr=binned["std_speedup"],
            marker='o', capsize=4, label=f"{threads} threads"
        )
    plt.axhline(1.0, color='gray', linestyle='--')
    plt.xlabel("log2(Input Size)")
    plt.ylabel("Average Speedup (Sequential / OpenMP)")
    plt.title(title)
    plt.grid(True)
    plt.legend(title="Threads")
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_efficiency_vs_threads(df_seq, df_omp_dict, title, output_files: list | None = None):
    """
    Compare binned average efficiency for different thread counts on a single plot, with error bars.
    """
    plt.figure(figsize=(12, 6))
    for threads, df_omp in sorted(df_omp_dict.items()):
        merged = pd.merge(df_seq, df_omp, on="size", suffixes=("_seq", f"_omp_{threads}"))
        merged["speedup"] = merged["time_ms_seq"] / merged[f"time_ms_omp_{threads}"]
        merged["efficiency"] = merged["speedup"] / threads
        merged["log2_size"] = np.log2(merged["size"]).astype(int)
        binned = merged.groupby("log2_size").agg(
            avg_efficiency=("efficiency", "mean"),
            std_efficiency=("efficiency", "std"),
            count=("efficiency", "count")
        ).reset_index()
        plt.errorbar(
            binned["log2_size"], binned["avg_efficiency"], yerr=binned["std_efficiency"],
            marker='o', capsize=4, label=f"{threads} threads"
        )
    plt.axhline(1.0, color='red', linestyle='--', label="100% Efficiency")
    plt.xlabel("log2(Input Size)")
    plt.ylabel("Average Efficiency (Speedup / Threads)")
    plt.title(title)
    plt.ylim(0, 1.0)
    plt.grid(True)
    plt.legend(title="Threads")
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


def plot_execution_time_binned(df, title, output_files: list | None = None):
    """
    Plot binned average execution time for different benchmarks.
    Bins by log2(size) and shows mean and std per label.
    """
    df = df.copy()
    df["log2_size"] = np.log2(df["size"]).astype(int)
    binned = df.groupby(["label", "log2_size"]).agg(
        avg_time_ms=("time_ms", "mean"),
        std_time_ms=("time_ms", "std"),
        count=("time_ms", "count")
    ).reset_index()

    plt.figure(figsize=(10, 5))
    for label, group in binned.groupby("label"):
        plt.errorbar(
            group["log2_size"], group["avg_time_ms"], yerr=group["std_time_ms"],
            marker='o', capsize=4, label=label
        )
    plt.xlabel("log2(Input Size)")
    plt.ylabel("Average Execution Time (ms)")
    plt.title(title)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    if output_files:
        for output_file in output_files:
            plt.savefig(output_file)
    plt.show()


if __name__ == "__main__":
    import thinkpad
    thinkpad.main()
