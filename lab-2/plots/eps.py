import pandas as pd
import matplotlib.pyplot as plt
import os

df = pd.read_csv("../data/table.csv", delimiter=";")

algorithms = ["NFDH", "FFDH"]
n_values = [1024]

plt.figure(figsize=(10, 5))

for alg in algorithms:
    for n in n_values:
        filtered_df = df[(df["algorithm"] == alg) & (df["n"] == n)]
        plt.plot(filtered_df["m"], filtered_df["eps"], marker='o', linestyle='-', label=f"{alg}, n={n}")

plt.xlim(500, 5000)
# plt.ylim(0, 1000)
plt.xlabel("Количество задач (m)")
plt.ylabel("ε")
plt.title("Зависимость ε от количества задач")
plt.legend()
plt.grid()
plt.savefig("../img/eps_test.png", dpi=300, bbox_inches="tight")
