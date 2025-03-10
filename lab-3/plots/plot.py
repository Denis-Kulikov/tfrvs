import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../data/table.csv", delimiter=";")

plt.figure(figsize=(10, 5))
plt.plot(df["n"], df["time_ms"], marker='o', linestyle='-', color='b', label="Время работы (ms)")

plt.xlabel("Размерность n")
plt.xlim(0, 500)
plt.ylabel("Время выполнения (мс)")
plt.title("Зависимость времени работы от n")
plt.legend()
plt.grid()

plt.savefig("../img/speed_test.png", dpi=300, bbox_inches="tight")
