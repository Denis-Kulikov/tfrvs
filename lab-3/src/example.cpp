#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <random>

#include <windows.h>

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

    void start(int iterations_, int start_index = -1) {
        V = 0;
        V_prev = numeric_limits<double>::infinity();
        iterations = 0;
        iterations_max = iterations_;
        int index = start_index < 0 ? rand() % I : start_index;

        Y = vector<double>(I, 0);
        y = vector<double>(I, 0);
        X = vector<double>(J, 0);
        x = vector<double>(J, 0);

        parent = this;
        if (results == nullptr) {
            results = new vector<result>;
        } else {
            results->clear();
        }
        
        callback(index);
    }

    ~brown_method() {
        if (parent == this && results != nullptr)
            delete results;
    }

    vector<result>* results = nullptr;

private:
    bool is_done() {
        calculate_V();
        return iterations >= iterations_max;
    }

    void callback(int index) {
        do {
            ++iterations;
            index = first(index);
        } while (!is_done()); // do {...} while(); - потому что iterations == 0 при запуске
        auto x_ = get_x();
        auto y_ = get_y();
        results->emplace_back(x_, y_, V, iterations);
    }

    int first(int index) {
        for (int j = 0; j < J; ++j) 
            X[j] += C[index][j];

        double min_value = *std::min_element(X.begin(), X.end());

        vector<int> indices;
        for (int j = 0; j < J; ++j) {
            if (X[j] == min_value) indices.push_back(j);
        }

        for (size_t i = 1; i < indices.size(); ++i) {
            brown_method bt(*this);
            bt.first_alternative(indices[i]);
        }

        int main_index = indices[0];
        ++x[main_index];
        return second(main_index);
    }

    int second(int index) {
        for (int i = 0; i < I; ++i)
            Y[i] += C[i][index];

        double max_value = *std::max_element(Y.begin(), Y.end());

        vector<int> indices;
        for (int i = 0; i < I; ++i) {
            if (Y[i] == max_value) indices.push_back(i);
        }

        for (size_t i = 1; i < indices.size(); ++i) {
            brown_method bt(*this);
            bt.second_alternative(indices[i]);
        }

        int main_index = indices[0];
        ++y[main_index];
        return main_index;
    }

    void first_alternative(int index) {
        ++x[index];
        index = second(index);
        if (!is_done()) callback(index);
    }

    void second_alternative(int index) {
        ++y[index];
        if (!is_done()) callback(index);
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
    int iterations_max;
    int I;
    int J;
    int iterations;
    brown_method* parent = nullptr;
};


int main() {
    SetConsoleOutputCP(65001);  
    srand(time(nullptr));

    vector<vector<double>> C;
    C = {{2, 0, 9, 6}, {1, 3, 6, 0}, {4, 2, 1, 3}};
    print_matrix(C);

    brown_method bm(C);
    bm.start(10, 2);
    auto results = *bm.results;

    for (int i = 0; i < results.size(); ++i) {
        cout << "iterations ≈ " << results[i].iterations << endl;
        cout << "V ≈ " << results[i].V << endl;
        cout << "Oптимaльныe cмeшaнныe cтpaтeгии" << endl;
        cout << "ВЦ: = ";
        for (const auto& it : results[i].y)
            cout << it << " ";
        cout << endl;
        cout << "Диcпeтчep: = ";
        for (const auto& it : results[i].x)
            cout << it << " ";
        cout << endl;
        cout << endl;
    }

    return 0;
}
