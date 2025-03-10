import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import random

def read_tasks(filename):
    """
    Считывает задачи из файла:
      j: <число>
      t: <число>
      𝜏: <число>
      machines: <m1> <m2> ...
    с пустой строкой после каждой задачи.
    """
    tasks = []
    with open(filename, "r", encoding="utf-8") as f:
        lines = [line.strip() for line in f if line.strip()]

    # Каждая задача занимает 4 строки (j, t, τ, machines).
    # Шагаем по 4 строки за раз.
    for i in range(0, len(lines), 4):
        # 1) j: 1
        j_line = lines[i]
        # 2) t: 5
        t_line = lines[i+1]
        # 3) 𝜏: 0
        tau_line = lines[i+2]
        # 4) machines: 1 1
        machines_line = lines[i+3]

        j_val = int(j_line.split(":")[1])
        t_val = int(t_line.split(":")[1])
        tau_val = int(tau_line.split(":")[1])
        machines_str = machines_line.split(":")[1].split()
        machines_list = list(map(int, machines_str))

        tasks.append({
            "j": j_val,
            "t": t_val,
            "tau": tau_val,
            "machines": machines_list
        })

    return tasks

def plot_tasks(tasks):
    """
    Строит «прямоугольники» на плоскости:
      - X = τ (start), width = t
      - Y = min(machines), height = len(machines)
      - Цвет зависит от j
    """
    fig, ax = plt.subplots(figsize=(8, 5))

    unique_js = sorted(set(task["j"] for task in tasks))
    color_map = {}
    for j in unique_js:
        color_map[j] = (random.random(), random.random(), random.random())

    for task in tasks:
        j = task["j"]
        t = task["t"]         
        tau = task["tau"]     
        machines = task["machines"]
    
        y = min(machines) if machines else 0
        h = len(machines)
        

        color = color_map[j]

        rect = Rectangle(
            (tau, y),  
            t,         
            h,         
            facecolor=color,
            edgecolor="black",
            label=f"Task {j}"
        )
        ax.add_patch(rect)

    ax.set_xlabel("Time (t)")
    ax.set_ylabel("Machines")
    ax.set_title("Tasks Visualization")

    max_t = max(task["tau"] + task["t"] for task in tasks) if tasks else 1
    ax.set_xlim(0, max_t + 1)
    max_m = 0
    for task in tasks:
        if task["machines"]:
            y_val = min(task["machines"])
            h_val = len(task["machines"])
            if y_val + h_val > max_m:
                max_m = y_val + h_val
    ax.set_ylim(0, max_m + 1)

    handles = []
    for j in unique_js:
        patch = Rectangle((0, 0), 1, 1, color=color_map[j], label=f"Task {j}")
        handles.append(patch)
    ax.legend(handles=handles, loc="upper right")

    plt.ylim(1, 6.999)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.show()

if __name__ == "__main__":
    tasks = read_tasks("ffdh.txt")
    for task in tasks:
        print(task["machines"])
    plot_tasks(tasks)
