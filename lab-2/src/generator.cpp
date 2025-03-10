#include "include/generator.hpp"

void generate_task_file(int n, int m, int t, const std::string& output_dir) {
    std::ofstream NFDH(output_dir + "NFDH" + "_n" + std::to_string(n) + "_m" + std::to_string(m) + "_t" + std::to_string(t) + ".txt");
    std::ofstream FFDH(output_dir + "FFDH" + "_n" + std::to_string(n) + "_m" + std::to_string(m) + "_t" + std::to_string(t) + ".txt");
    if (!NFDH.is_open()) {
        std::cerr << "Error: couldn't create file " << "NFDH" << "_" << m << ".txt" << std::endl;
        return;
    }
    if (!FFDH.is_open()) {
        std::cerr << "Error: couldn't create file " << "FFDH" << "_" << m << ".txt" << std::endl;
        return;
    }

    NFDH << n << "\n";
    NFDH << "NFDH" << "\n";
    FFDH << n << "\n";
    FFDH << "FFDH" << "\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_r(1, n);
    std::uniform_int_distribution<int> dist_t(1, 100);

    for (int i = 0; i < m; ++i) {
        int t = dist_t(gen);
        int r = dist_r(gen);
        NFDH << t << " " << r << "\n";
        FFDH << t << " " << r << "\n";
    }
}