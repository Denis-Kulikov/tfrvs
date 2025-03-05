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
    xlabel="Количество машин (n)" 
    ylabel="Θ(n), часов"
    title="Среднее время наработки до отказа Θ(n)"

    num_points = len(data[0]) 
    x_values = np.arange(65526, 65526 + num_points)

    plt.figure(figsize=(12, 7))

    for i, row in enumerate(data):
        plt.plot(x_values, row, marker='o', linestyle='-', label=f"μ={10**i}")

    plt.xscale("linear")
    plt.xlim(65526, 65526 + num_points - 1)
    plt.yscale("log")
    plt.ylim(1, 262144)

    y_ticks = [4**i for i in range(10)]
    plt.yticks(y_ticks, labels=[str(y) for y in y_ticks])

    plt.grid(True, which="major", linestyle="--", linewidth=0.75)
    plt.minorticks_off()

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    
    plt.savefig("../img/1-1-theta_mu.png", dpi=300, bbox_inches="tight")

data = read_data("../data/theta_mu.txt")
plot_graph(data)
