#include "include/parser.hpp"

bool readParams(const std::string& filename, int& N, int& m, double& lambda, double& mu, std::vector<int>& n_values) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    file >> N >> m >> lambda >> mu;

    int n;
    while (file >> n) {
        n_values.push_back(n);
    }

    return true;
}
