import matplotlib.pyplot as plt
import numpy as np

def read_data(filename):
    data = []
    with open(filename, 'r') as file:
        for line in file:
            values = list(map(float, line.split()))
            data.append(values)
    return np.array(data)

def plot_graph(data):
    xlabel="x" 
    ylabel="y"
    title="R*(t)"

    num_points = len(data[0]) 
    x_values = np.arange(0, 24 + 1, 2)

    plt.figure(figsize=(12, 7))

    for i, row in enumerate(data):
        plt.plot(x_values, row, marker='o', linestyle='-', label=f"n={8+i}")

    plt.xscale("linear")
    plt.xlim(0, 25)

    plt.grid(True, which="major", linestyle="--", linewidth=0.75)
    plt.minorticks_off()

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    
    plt.savefig("../img/r_star.png", dpi=300, bbox_inches="tight")

data = read_data("../data/r_star.txt")
plot_graph(data)
