#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "include/task.hpp"

bool parse_file(const std::string& filename, int& n, std::string& algorithm, std::vector<Task>& tasks) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: couldn't open the file " << filename << std::endl;
        return false;
    }

    if (!(file >> n)) {
        std::cerr << "Error: invalid value of n." << std::endl;
        return false;
    }

    if(!(file >> algorithm)) {
        std::cerr << "Error: algorithm" << std::endl;
        return false;
    }

    if (algorithm != "NFDH" && algorithm != "FFDH") {
        std::cerr << "Error: The algorithm (" << algorithm << ") must be 'NFDH' or 'FFDH'." << std::endl;
        return false;
    }

    tasks.clear();
    tasks.reserve(n);
    int m = 0;
    int t, r;
    while (file >> t >> r) {
        tasks.emplace_back(t, r, m++);
    }

    return true;
}