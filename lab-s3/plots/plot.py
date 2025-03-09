import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

df = pd.read_csv("../data/table.csv", delimiter=";")

df["theta"] = df["theta"].apply(lambda x: np.array([float(i) for i in x.split(",")]))
df["T"] = df["T"].apply(lambda x: np.array([float(i) for i in x.split(",")])) 

fun = ["theta", "T"]
lambda_array = [1e-6, 1e-7, 1e-5]
mu_array = [1, 10, 100, 1000]
m_array = [1, 2, 3]
n_array = np.arange(65527, 65537)

def plot_and_save(x_values, y_values_list, labels, y_label, title, func, dep, fixed_params):
    plt.figure(figsize=(10, 5))
    for y_values, label in zip(y_values_list, labels):
        plt.plot(n_array, y_values, marker='o', linestyle='-', label=label)

    plt.xlim(65527, 65536)
    plt.xlabel("n")
    plt.ylabel(y_label)
    if func == "theta":
        plt.yscale("log")

    plt.title(title)
    plt.legend()
    plt.grid()

    file_path = f"../img/{func}/{dep}/" + "_".join([f"{k}_{v}" for k, v in fixed_params.items()]) + ".png"
    os.makedirs(os.path.dirname(file_path), exist_ok=True)
    
    plt.savefig(file_path, dpi=200, bbox_inches="tight")
    plt.close()

for f in fun:
    for mu_fixed in mu_array:
        for m_fixed in m_array:
            filtered_df = df[(df["mu"] == mu_fixed) & (df["m"] == m_fixed)]
            if not filtered_df.empty:
                y_values_list = [
                    filtered_df[filtered_df["lambda"] == lam][f].values[0]  
                    for lam in lambda_array
                    if not filtered_df[filtered_df["lambda"] == lam].empty
                ]
                plot_and_save(
                    x_values=lambda_array,
                    y_values_list=y_values_list,
                    labels=[f"λ={lam}" for lam in lambda_array],
                    y_label=f,
                    title=f"Зависимость {f} от λ при μ={mu_fixed}, m={m_fixed}",
                    func=f,
                    dep="lambda",
                    fixed_params={"mu": mu_fixed, "m": m_fixed}
                )

    for lambda_fixed in lambda_array:
        for m_fixed in m_array:
            filtered_df = df[(df["lambda"] == lambda_fixed) & (df["m"] == m_fixed)]
            if not filtered_df.empty:
                y_values_list = [
                    filtered_df[filtered_df["mu"] == mu][f].values[0]  
                    for mu in mu_array
                    if not filtered_df[filtered_df["mu"] == mu].empty
                ]
                plot_and_save(
                    x_values=mu_array,
                    y_values_list=y_values_list,
                    labels=[f"μ={mu}" for mu in mu_array],
                    y_label=f,
                    title=f"Зависимость {f} от μ при λ={lambda_fixed}, m={m_fixed}",
                    func=f,
                    dep="mu",
                    fixed_params={"lambda": lambda_fixed, "m": m_fixed}
                )

    for lambda_fixed in lambda_array:
        for mu_fixed in mu_array:
            filtered_df = df[(df["lambda"] == lambda_fixed) & (df["mu"] == mu_fixed)]
            if not filtered_df.empty:
                y_values_list = [
                    filtered_df[filtered_df["m"] == m][f].values[0]  
                    for m in m_array
                    if not filtered_df[filtered_df["m"] == m].empty
                ]
                plot_and_save(
                    x_values=m_array,
                    y_values_list=y_values_list,
                    labels=[f"m={m}" for m in m_array],
                    y_label=f,
                    title=f"Зависимость {f} от m при λ={lambda_fixed}, μ={mu_fixed}",
                    func=f,
                    dep="m",
                    fixed_params={"lambda": lambda_fixed, "mu": mu_fixed}
                )
