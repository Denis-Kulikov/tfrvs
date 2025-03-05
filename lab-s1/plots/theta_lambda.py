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
        plt.plot(x_values, row, marker='o', linestyle='-', label=f"λ={10**(-i - 5)}")

    plt.xscale("linear")
    plt.xlim(65526, 65526 + num_points - 1)
    plt.yscale("log")

    plt.grid(True, which="major", linestyle="--", linewidth=0.75)
    plt.minorticks_off()

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    
    plt.savefig("../img/1-2-theta_lambda.png", dpi=300, bbox_inches="tight")

data = read_data("../data/theta_lambda.txt")
plot_graph(data)
