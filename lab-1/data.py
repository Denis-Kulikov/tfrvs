import matplotlib.pyplot as plt

sizes = [1, 32, 64, 96, 128, 160, 192]
ram1 = [0.000008, 0.000018, 0.000030, 0.000031, 0.000040, 0.000046, 0.000051]
ram2 = [0.000004, 0.000011, 0.000017, 0.000020, 0.000025, 0.000030, 0.000036]
numa = [0.000004, 0.000013, 0.000021, 0.000026, 0.000036, 0.000043, 0.000047]
infiniband = [0.000082, 0.000099, 0.000118, 0.000131, 0.000283, 0.000288, 0.000295]
		
plt.figure(figsize=(10, 6))
plt.plot(sizes, ram1, marker='o', label='RAM (1 core)')
plt.plot(sizes, ram2, marker='v', label='RAM (2 core)')
plt.plot(sizes, numa, marker='s', label='NUMA')
plt.plot(sizes, infiniband, marker='^', label='InfiniBand')

plt.xlabel('Размер сообщения (МБ)')
plt.ylabel('Время передачи (с)')
plt.title('Зависимость времени передачи от размера сообщения')
plt.legend()
plt.grid(True)

plt.xticks(sizes)

plt.show()
