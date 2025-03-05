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

    x_values = np.arange(8092, 8192 + 1, 10)

    plt.figure(figsize=(12, 7))

    for i, row in enumerate(data):
        plt.plot(x_values, row, marker='o', linestyle='-', label=f"λ={10**(-i - 5)}")

    plt.xlim(8090, 8200)
    plt.ylim(0.95, 1.2)

    plt.grid(True, which="major", linestyle="--", linewidth=0.75)
    plt.minorticks_off()

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    
    plt.savefig("../img/2-2-t_lambda.png", dpi=300, bbox_inches="tight")

data = read_data("../data/t_lambda.txt")
plot_graph(data)
