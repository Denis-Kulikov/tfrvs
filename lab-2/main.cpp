#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>

#include "task.hpp"
#include "ffdh.hpp"
#include "nfdh.hpp"


int main() {
    std::vector<Task> tasks;
    std::string algorithm;
    int n;

    if (!parse_file(std::string("config_NHDH.txt"), n, algorithm, tasks)) return -1;
    // if (!parse_file(std::string("config_FFDH.txt"), n, algorithm, tasks)) return -1;
    
    std::cout << "algorithm: " << algorithm << std::endl;
    if (algorithm == "NHDH") {
        NFDH nfdh(n);
        nfdh.start(tasks);
    } else {
        FFDH ffdh(n);
        ffdh.start(tasks);
        ffdh.printTree();
    }

    for (const auto &task : tasks) {
        std::cout << "j: " << task.j + 1 << "\n";       // номер задачи
        std::cout << "t: " << task.t << "\n";           // время выполнения задачи
        std::cout << "𝜏: " << task.schedule.tau << "\n";  // время начала выполнения задачи
        std::cout << "machines: ";                      // номера выделенных машин
        for (const auto& x : task.schedule.x)  {
            std::cout << (x + 1) << " ";
        }
        std::cout << "\n" << std::endl;
    }

    return 0;
}
