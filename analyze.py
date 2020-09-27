#!/usr/bin/python3

from collections import defaultdict
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 16})



seq_times = defaultdict(list)
distr_times = defaultdict(list)

for n in (3, 6, 8):
    for p in range(1, 9):
        with open('logs/log-{0}-{1}.txt'.format(p, n)) as log:
            lines = log.readlines()
            seq_times[n].append(float(lines[3].split()[2]))
            distr_times[n].append(float(lines[2].split()[2]))



seq_time_means = {n: np.mean(seq_times[n]) for n in (3, 6, 8)}
plt.figure(figsize=(16, 9))
plt.suptitle('Время последовательного подсчёта интеграла основным процессом, с')

for subplot_index, n in zip((1, 2, 3), (3, 6, 8)):
    plt.subplot(1, 3, subplot_index)
    plt.bar(np.arange(8), seq_times[n], width=1, alpha=0.5)
    plt.hlines(seq_time_means[n], -1, 8, colors='green', 
        label='Среднее значение')
    plt.title('Число отрезков N = $10^{}$'.format(n), y=1.02)
    plt.xlim(-1, 8)
    plt.xticks([])
    plt.grid()
    plt.legend()

plt.savefig('sequential-time.png', dpi=300)



plt.figure(figsize=(16, 9))
plt.suptitle('Время параллельного подсчёта интеграла, с')

for subplot_index, n in zip((1, 2, 3), (3, 6, 8)):
    plt.subplot(1, 3, subplot_index)
    plt.bar(np.arange(8), distr_times[n], width=1, alpha=0.5)
    plt.title('Число отрезков N = $10^{}$'.format(n))
    plt.xlim(-1, 8)
    plt.xticks([])
    plt.grid()

plt.savefig('distributed-time.png', dpi=300)



plt.figure(figsize=(16, 9))
s = {n: seq_time_means[n] / distr_times[n] for n in (3, 6, 8)}

for n in (3, 6, 8):
    plt.plot(range(1, 9), s[n], linewidth=2,
        label='Число отрезков N = $10^{}$'.format(n))

plt.title('Зависимость ускорения от количества процессов для разных разбиений')
plt.xlabel('p, количество процессов')
plt.ylabel('S, ускорение')
plt.xlim(1, 8)
plt.grid()
plt.legend()
plt.savefig('acceleration.png', dpi=300)



times = []

for i in (2, 3, 4, 6):
    with open('logs/log-{}.txt'.format(i)) as log:
        lines = log.readlines()
        times.append(float(lines[2].split()[1]))

plt.figure(figsize=(16, 9))
plt.plot((2, 3, 4, 6), times)
plt.title('Зависимость времени вычислений от отношения MPI-процессов и потоков')
plt.xlabel('Количество MPI-процессов')
plt.ylabel('Время вычислений, с')
plt.xticks((2, 3, 4, 6))
plt.grid()
plt.savefig('openmp.png', dpi=300)
