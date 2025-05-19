import json
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
from typing import List, Dict

# Configure style
sns.set(style="whitegrid")
plt.rcParams["figure.figsize"] = (12, 6)


# ---------- Load all JSON files ----------
def load_benchmark_data(folder: str) -> List[Dict]:
    all_data = []
    for file in Path(folder).glob("*.json"):
        with open(file) as f:
            data = json.load(f)
            for bench in data["benchmarks"]:
                all_data.append({
                    "implementation": implementation_from_name(file.name),
                    "dimension": dimension_from_name(bench["name"]),
                    "name": bench["name"],
                    "time_ns": bench["real_time"],
                    "threads": bench["threads"]
                })
    return all_data


# ---------- Helpers to extract labels ----------
def implementation_from_name(filename: str) -> str:
    if "sequential" in filename:
        return "Sequential"
    elif "openmp" in filename:
        return "OpenMP"
    elif "cuda" in filename:
        return "CUDA"
    else:
        return "Unknown"


def dimension_from_name(name: str) -> str:
    if "1D" in name:
        return "1D"
    elif "2D" in name:
        return "2D"
    elif "3D" in name:
        return "3D"
    else:
        return "Unknown"


# ---------- Plot by Implementation ----------
def plot_by_implementation(data):
    sns.barplot(
        x="implementation", y="time_ns", hue="dimension",
        data=data, errorbar=None
    )
    plt.yscale("log")
    plt.title("FFT Performance by Implementation (log scale)")
    plt.ylabel("Execution Time (ns)")
    plt.xlabel("Implementation")
    plt.legend(title="Dimension")
    plt.tight_layout()
    plt.show()


# ---------- Plot by Dimension ----------
def plot_by_dimension(data):
    sns.barplot(
        x="dimension", y="time_ns", hue="implementation",
        data=data, errorbar=None
    )
    plt.yscale("log")
    plt.title("FFT Performance by Dimension (log scale)")
    plt.ylabel("Execution Time (ns)")
    plt.xlabel("Dimension")
    plt.legend(title="Implementation")
    plt.tight_layout()
    plt.show()


# ---------- Compute Speedup ----------
def compute_speedup(data: pd.DataFrame) -> pd.DataFrame:
    speedup_data = []
    for dim in data["dimension"].unique():
        base = data[(data["dimension"] == dim) & (data["implementation"] == "Sequential")]
        for _, base_row in base.iterrows():
            base_time = base_row["time_ns"]
            for impl in ["OpenMP", "CUDA"]:
                match = data[
                    (data["dimension"] == dim) &
                    (data["implementation"] == impl) &
                    (data["name"] == base_row["name"])
                ]
                if not match.empty:
                    other_time = match.iloc[0]["time_ns"]
                    speedup_data.append({
                        "dimension": dim,
                        "name": base_row["name"],
                        "implementation": impl,
                        "speedup": base_time / other_time
                    })
    return pd.DataFrame(speedup_data)


# ---------- Plot Speedup ----------
def plot_speedup(df_speedup):
    sns.barplot(x="dimension", y="speedup", hue="implementation", data=df_speedup)
    plt.title("Speedup over Sequential Implementation")
    plt.ylabel("Speedup (X times faster)")
    plt.xlabel("Dimension")
    plt.axhline(1, ls="--", color="gray")
    plt.tight_layout()
    plt.show()


# ---------- Main ----------
if __name__ == "__main__":
    folder = "./thinkpad"
    data = load_benchmark_data(folder)

    # Convert to DataFrame for seaborn
    df = pd.DataFrame(data)

    df_speedup = compute_speedup(df)

    # Plots
    plot_speedup(df_speedup)
    plot_by_implementation(df)
    plot_by_dimension(df)
