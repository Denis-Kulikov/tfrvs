#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "task.hpp"

bool parse_file(const std::string& filename, int& n, std::string& algorithm, std::vector<Task>& tasks) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    if (!(file >> n)) {
        std::cerr << "Ошибка: некорректное значение n." << std::endl;
        return false;
    }

    if (!(file >> algorithm) || (algorithm != "NHDH" && algorithm != "FFDH")) {
        std::cerr << "Ошибка: алгоритм должен быть 'NFDH' или 'FFDH'." << std::endl;
        return false;
    }

    tasks.reserve(n);
    int t, r;
    while (file >> t >> r) {
        tasks.emplace_back(t, r);
    }

    return true;
}