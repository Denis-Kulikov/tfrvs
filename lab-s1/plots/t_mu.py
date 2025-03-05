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
    ylabel="T(n), часов"
    title="Среднее время восстановления T(n)"

    x_values = np.arange(900, 1000 + 1, 10)
    mu_values = [1, 2, 4, 6]

    plt.figure(figsize=(12, 7))

    for i, row in enumerate(data):
        plt.plot(x_values, row, marker='o', linestyle='-', label=f"μ={mu_values[i]}")

    plt.xscale("linear")
    plt.xlim(900, 1000)
    plt.yscale("log")
    plt.ylim(0.125, 64.0)

    y_ticks = [2**(i - 3) for i in range(10)]
    plt.yticks(y_ticks, labels=[str(y) for y in y_ticks])

    plt.grid(True, which="major", linestyle="--", linewidth=0.75)
    plt.minorticks_off()

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    
    plt.savefig("../img/2-1-t_mu.png", dpi=300, bbox_inches="tight")

data = read_data("../data/t_mu.txt")
plot_graph(data)
