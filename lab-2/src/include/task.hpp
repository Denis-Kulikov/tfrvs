#pragma once
#include <iostream>
#include <vector>

class Task {
public:
    Task(int t, int r, int m)
        : t(t), r(r), schedule(r), j(m) {}

    class Schedule {
    public:
        Schedule(int r)
            : x(r, -2)
        {}

        std::vector<int> x;    // номера ЭМ, на которые распределены ветки
        int tau;               // время старта задачи
    };

    int t; // время решения 
    int r; // количеством необходимых элементарных машин
    int j; // ∈ {1, ..., m]
    Schedule schedule;
};

std::vector<Task> countingSort(std::vector<Task>& tasks);
bool parse_file(const std::string& filename, int& n, std::string& algorithm, std::vector<Task>& tasks);
