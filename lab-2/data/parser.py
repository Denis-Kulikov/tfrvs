import re
from datetime import datetime

# Функция для вычисления разницы во времени
def parse_time_difference(start_time, end_time):
    fmt = "%Y-%m-%dT%H:%M:%S"
    t1 = datetime.strptime(start_time, fmt)
    t2 = datetime.strptime(end_time, fmt)
    return int((t2 - t1).total_seconds())  # Округляем до целого

# Регулярное выражение для извлечения данных
log_pattern = re.compile(
    r"StartTime=(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}) "
    r"EndTime=(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}) .*?"
    r"NodeCnt=(\d+)"
)

# Количество узлов в LLNL Atlas
n = 1152
algorithms = ["NFDH", "FFDH"]
m_values = [500, 1000, 1500]

# Открываем файлы и записываем заголовки
file_handles = {}
task_counters = {}

for alg in algorithms:
    for m in m_values:
        filename = f"parsed_{alg}_m{m}.txt"
        file_handles[(alg, m)] = open(filename, "w")
        file_handles[(alg, m)].write(f"{n}\n{alg}\n")
        task_counters[(alg, m)] = 0  # Инициализируем счётчик

# Читаем лог-файл и распределяем задачи
log_filename = "LLNL-Atlas-2006-0"

with open(log_filename, "r") as log_file:
    for line in log_file:
        match = log_pattern.search(line)
        if match:
            start_time, end_time, node_cnt = match.groups()
            t = parse_time_difference(start_time, end_time)
            r = int(node_cnt)
            
            if r == 0 or t == 0:
                continue  # Пропускаем некорректные данные

            for alg in algorithms:
                for m in m_values:
                    if task_counters[(alg, m)] < m:  # Ограничение количества записей
                        file_handles[(alg, m)].write(f"{t} {r}\n")
                        task_counters[(alg, m)] += 1

# Закрываем файлы
for f in file_handles.values():
    f.close()

print("Данные записаны в 6 файлов корректно.")
