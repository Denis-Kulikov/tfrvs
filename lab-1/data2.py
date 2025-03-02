import matplotlib.pyplot as plt

sizes = [1, 32, 64, 96, 128, 160, 192]
n1_8 = [0.000167, 0.000173, 0.000181, 0.000280, 0.000315, 0.000477, 0.000716]
n2_4 = [0.000208, 0.000291, 0.000563, 0.000717, 0.000892, 0.001183, 0.001271]
		
plt.figure(figsize=(10, 6))
plt.plot(sizes, n1_8, marker='o', label='1 * 8')
plt.plot(sizes, n2_4, marker='v', label='2 * 4')

plt.xlabel('Размер сообщения (МБ)')
plt.ylabel('Время передачи (с)')
plt.title('Зависимость времени передачи от размера сообщения')
plt.legend()
plt.grid(True)

plt.xticks(sizes)

plt.show()
