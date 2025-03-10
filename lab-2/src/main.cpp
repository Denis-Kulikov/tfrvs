#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>

#include "include/task.hpp"
#include "include/ffdh.hpp"
#include "include/nfdh.hpp"
#include "include/generator.hpp"

const int test_count = 10;
std::vector<std::string> algorithms = {"NFDH", "FFDH"};
std::vector<int> n_values = {1024, 4096};
std::vector<int> m_values = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};


struct Result {
    double time_ms;
    int T_S;
    double eps;
};

Result mean(const std::vector<Result>& r) {
    Result m = {0, 0, 0};
    for (const auto& res : r) {
        m.time_ms += res.time_ms;
        m.T_S     += res.T_S;
        m.eps     += res.eps;
    }

    m.time_ms /= r.size();
    m.T_S     /= r.size();
    m.eps     /= r.size();

    return m;
}

double sigma(std::vector<Result>& r, double mean_eps) {
    double s = 0;
    for (std::size_t i = 0; i < r.size(); ++i) {
        double diff = r[i].eps - mean_eps;
        s += diff * diff;
    }
    return std::sqrt(s / r.size());
}


Result run(const std::string& filename, std::vector<Task>& tasks) {
    std::string algorithm;
    int n;

    if (!parse_file(filename, n, algorithm, tasks)) return {0, 0, 0};

    auto start = std::chrono::high_resolution_clock::now();
    if (algorithm == "NFDH") {
        NFDH nfdh(n);
        nfdh.start(tasks);
    } else {
        FFDH ffdh(n);
        ffdh.start(tasks);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    int T_S = 0;
    for (const auto& task : tasks) {
        double finish_time = task.schedule.tau + task.t;
        if (finish_time > T_S) {
            T_S = finish_time;
        }
    }

    double T_min = 0; // T' = (1/n) * Σ (r_j * t_j)
    for (const auto& task : tasks) {
        T_min += task.r * task.t;
    }
    T_min /= n;

    double eps = (T_S - T_min) / T_min;

    return {elapsed.count(), T_S, eps};
}

void example() {
    std::vector<Task> tasks1, tasks2;
    std::vector<std::string> alg = {"../data/config_NFDH.txt", "../data/config_FFDH.txt"};
    std::vector<std::vector<Task>> vec;
    Result result[] = {run(alg[0], tasks1), run(alg[1], tasks2)};
    vec.push_back(tasks1);
    vec.push_back(tasks2);

    int i = 0;
    for (const auto& v: vec) {
        std::cout << "alg: " << alg[i] << std::endl;
        for (const auto &task : v) {
            std::cout << "j: " << task.j + 1 << "\n";          // номер задачи
            std::cout << "t: " << task.t << "\n";              // время выполнения задачи
            std::cout << "tau: " << task.schedule.tau << "\n"; // время начала выполнения задачи
            std::cout << "machines: ";                         // номера выделенных машин
            for (const auto& x : task.schedule.x)
                std::cout << (x + 1) << " ";

            std::cout << "\n" << std::endl;
        }

        std::cout << "T_S: " << result[i].T_S << std::endl;
        std::cout << "time_ms: " << result[i].time_ms << std::endl;
        std::cout << "eps: " << result[i].eps << std::endl;
        std::cout << "\n" << std::endl;
        ++i;
    }
}

void test_LLNL_Atlas() {
    std::ofstream table("../data/LLNL_Atlas.csv");
    if (!table.is_open()) {
        std::cerr << "Error: couldn't open the file '../data/LLNL_Atlas.csv'" << std::endl;
        return;
    }
    table << "algorithm;n;m;time_ms;T_S;eps;sigma_eps\n";

    std::vector<int> m_values = {500, 1000, 1500};
    std::vector<Task> tasks;
    for (const auto& alg : algorithms) {
        for (int m : m_values) {
            std::vector<Result> res(10);
            for (int t = 0; t < test_count; t++) {
                std::string filename = "../data/parsed_" + alg + "_m" + std::to_string(m) + ".txt";
                res[t] = run(filename, tasks);
            }
            Result r = mean(res);
            double s = sigma(res, r.eps);

            table << alg << ";" << "1152" << ";" << m << ";" << r.time_ms << ";" << r.T_S << ";" << r.eps << ";" << s << "\n";
        }
    }

    table.close();
}

int test() {
    std::ofstream table("../data/table.csv");
    if (!table.is_open()) {
        std::cerr << "Error: couldn't open the file '../data/table.csv'" << std::endl;
        return -1;
    }
    table << "algorithm;n;m;time_ms;T_S;eps;sigma_eps\n";

    std::vector<Task> tasks;
    for (const auto& alg : algorithms) {
        for (int n : n_values) {
            for (int m : m_values) {
                std::vector<Result> res(10);
                for (int t = 0; t < test_count; t++) {
                    std::string filename = "tasks/" + alg + "_n" + std::to_string(n) + "_m" + std::to_string(m) + "_t" + std::to_string(t) + ".txt";
                    res[t] = run(filename, tasks);
                }
                Result r = mean(res);
                double s = sigma(res, r.eps);
                table << alg << ";" << n << ";" << m << ";" << r.time_ms << ";" << r.T_S << ";" << r.eps << ";" << s << "\n";
            }
        }
    }

    table.close();
    return 0;
}

void generate() {
    for (const auto& alg : algorithms) {
        for (int n : n_values) {
            for (int m : m_values) {
                for (int t = 0; t < test_count; t++)
                    generate_task_file(n, m, t, "tasks/");
            }
        }
    }
}

int main() {
    // example();

    generate();
    test();

    // test_LLNL_Atlas();

    return 0;
}
