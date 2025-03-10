#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <limits>
#include <thread>
#include <mutex>
#include <random>

using namespace std;

vector<vector<double>> compute_payoff_matrix(int n, double c1, double c2, double c3) {
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) { // ВЦ 
        for (int j = 0; j < n; j++) { // Диспетчер
            if (i >= j)  C[i][j] = j * c1 + (i - j) * c2; // i >= j
            else         C[i][j] = i * c2 + (j - i) * c3; // i <  j
        }
    }

    return C;
}

void print_matrix(const vector<vector<double>>& C) {
    cout << "Matrix C:" << endl;
    for (int i = 0; i < C.size(); i++) {
        for (int j = 0; j < C[i].size(); j++) {
            cout << setw(8) << C[i][j] << " ";
        }
        cout << endl;
    }
}

class brown_method {
public:
    class result {
    public:
        result(vector<double> &x_, vector<double> &y_, double V_, int iterations_) {
            x = std::move(x_);
            y = std::move(y_);
            V = V_;
            iterations = iterations_;
        }

        vector<double> x, y;
        double V;
        int iterations;
    };

    brown_method(const vector<vector<double>>& C_)
        : C(C_) {
        I = C.size();
        J = C[0].size();
    }

    double get_V() { return V; }
    double get_iterations() { return iterations; }
    vector<double> get_x() {
        vector<double> x_(x.size());
        for (int j = 0; j < J; ++j)
            x_[j] = x[j] / iterations;

        return x_;
    }
    vector<double> get_y() {
        vector<double> y_(y.size());
        for (int i = 0; i < I; ++i)
            y_[i] = y[i] / iterations;

        return y_; 
    }

    void start(double epsilon_) {
        V = 0;
        V_prev = numeric_limits<double>::infinity();
        iterations = 0;
        epsilon = epsilon_;
        int index = rand() % I;

        Y = vector<double>(I, 0);
        y = vector<double>(I, 0);
        X = vector<double>(J, 0);
        x = vector<double>(J, 0);
        results.clear();
        
        callback(index);
    }

    vector<result> results;

private:
    bool is_done() {
        calculate_V();
        return fabs(V - V_prev) <= epsilon;
    }

    void callback(int index) {
        do {
            ++iterations;
            index = first(index);
        } while (!is_done()); // do {...} while(); - потому что iterations == 0 при запуске
        auto x_ = get_x();
        auto y_ = get_y();
        results.emplace_back(x_, y_, V, iterations);
    }

    int first(int index) {
        for (int j = 0; j < J; ++j) 
            X[j] += C[index][j];

        auto min_value = std::min_element(X.begin(), X.end());
        auto min_index = std::distance(X.begin(), min_value);

        ++x[min_index];
        return second(min_index);
    }

    int second(int index) {
        for (int i = 0; i < I; ++i)
            Y[i] += C[i][index];

        auto max_value = std::max_element(X.begin(), X.end());
        auto max_index = std::distance(X.begin(), max_value);

        ++y[max_index];
        return max_index;
    }

    void calculate_V() {
        auto x_ = get_x();
        auto y_ = get_y();
        V_prev = V;
        V = 0;

        for (int i = 0; i < I; ++i) {
            for (int j = 0; j < J; ++j) {
                V += C[i][j] * x_[j] * y_[i];
            }
        }
    }

    vector<vector<double>> C; 
    vector<double> X, Y;
    vector<double> x, y;
    double V;
    double V_prev;
    double epsilon;
    int I;
    int J;
    int iterations;
    brown_method* parent = nullptr;
};

double run(int n) {
    double c1 = 1.0, c2 = 2.0, c3 = 3.0, epsilon = 0.01;
    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<double>> C = compute_payoff_matrix(n, c1, c2, c3);

    brown_method bm(C);
    bm.start(epsilon);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    return elapsed.count();
}

void process_tests(const std::vector<int>& n_values, std::vector<double>& t_values, int test_count, int thread_id, int num_threads) {
    for (int i = thread_id; i < n_values.size(); i += num_threads) {
        for (int t = 0; t < test_count; ++t) {
            double time_ms = run(n_values[i]);
            t_values[i] += time_ms;
        }
    }
}

int main() {
    srand(time(nullptr));
    vector<int> n_values;
    for (int i = 10; i <= 500; i += 10) n_values.emplace_back(i);
    vector<double> t_values(n_values.size(), 0);

    std::ofstream file("../data/table.csv");
    if (!file.is_open()) {
        std::cerr << "Error: couldn't open '../data/table.csv'" << std::endl;
        return -1;
    }

    const int test_count = 5;
    const int num_threads = 10;
    std::vector<std::thread> threads;

    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        threads.emplace_back(process_tests, std::ref(n_values), std::ref(t_values), test_count, thread_id, num_threads);
    }

    for (auto& t : threads) {
        t.join();
    }

    file << "n;time_ms\n";
    
    for (int i = 0; i < n_values.size(); ++i) {
        file << n_values[i] << ";" << t_values[i] / test_count << "\n";
        std::cout << n_values[i] << ";" << t_values[i] / test_count << "\n";
    }

    file.close();

    return 0;
}
