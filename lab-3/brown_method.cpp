#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <random>

using namespace std;

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
    brown_method(const vector<vector<double>>& C_)
        : Ci(C_) {
        I = Ci.size();
        J = Ci[0].size();

        // Транспонируем матрицу
        for (size_t i = 0; i < Ci.size(); ++i) {
            for (size_t j = 0; j < Ci[i].size(); ++j) {
                Cj[j][i] = Ci[i][j];
            }
        }
    }

    double get_V() { return V; }
    double get_iterations() { return iterations; }
    vector<double> get_x() {
        std::cout << x.size() << std::endl;
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

    void start(double epsilon) {
        int index = 2;
        // int index = rand() % I;
        V = 0;
        V_prev = numeric_limits<double>::infinity();
        iterations = 0;
        Y = vector<double>(I, 0);
        y = vector<double>(I, 0);
        X = vector<double>(J, 0);
        x = vector<double>(J, 0);

        do {
            ++iterations;
            index = first(index);
            calculate_V_simple();

            for (int j = 0; j < J; ++j)
                std::cout << X[j] << " ";   
            std::cout << std::endl;
            for (int i = 0; i < I; ++i)
                std::cout << Y[i] << " ";   
            std::cout << std::endl;

            for (int j = 0; j < J; ++j)
                std::cout << x[j] << " ";   
            std::cout << std::endl;
            for (int i = 0; i < I; ++i)
                std::cout << y[i] << " ";   
            std::cout << std::endl;
            std::cout << std::endl;
        } while (iterations < 10); // do {...} while(); - потому что iterations == 0 при запуске
    

    }

private:
    int first(int index) {
        for (int j = 0; j < J; ++j) 
            X[j] += Ci[index][j];

        auto min_value = std::min_element(X.begin(), X.end());
        auto index_min = std::distance(X.begin(), min_value);
        ++x[index_min];

        return second(index_min);
    }

    int second(int index) {
        for (int i = 0; i < I; ++i)
            Y[i] += Ci[i][index];

        auto max_value = std::max_element(Y.begin(), Y.end());
        auto index_max = std::distance(Y.begin(), max_value);
        ++y[index_max];

        return index_max;
    }

    void calculate_V_simple() {
        V_prev = V;
        double min_value = *std::min_element(Y.begin(), Y.end());
        double max_value = *std::max_element(X.begin(), X.end());

        double offset = (max_value - min_value) / 2;
        double mid = min_value + offset;
        V = mid / iterations; // min_value / iterations <= V <= max_value / iterations
    }

    void calculate_V() {
        auto x_ = get_x();
        auto y_ = get_y();
        V_prev = V;
        double v = 0;

        for (int i = 0; i < I; ++i) {
            for (int j = 0; j < J; ++j) {
                v += Ci[i][j] * x_[j] * y_[i];
            }
        }

        V = v;
    }

    vector<vector<double>> Ci; // изначальная матрица
    vector<vector<double>> Cj; // транспонированная
    vector<double> X, Y;
    vector<double> x, y;
    double V;
    double V_prev;
    int I;
    int J;
    int iterations;
};

int main() {
    int n = 11;

    vector<vector<double>> Ci;
    vector<double> i1, i2, i3;
    i1 = {2, 0, 9, 6};
    i2 = {1, 3, 6, 0};
    i3 = {4, 2, 1, 3};

    Ci.push_back(i1);
    Ci.push_back(i2);
    Ci.push_back(i3);

    print_matrix(Ci);

    brown_method bm(Ci);
    bm.start(1);

    cout << "V ≈ " << bm.get_V() << endl;
    cout << "iterations = " << bm.get_iterations() << endl;
    // cout << "x = ";
    // for (const auto& it : bm.get_x())
    //     cout << it << " ";
    // cout << endl;
    // cout << "y = ";
    // for (const auto& it : bm.get_y())
    //     cout << it << " ";
    // cout << endl;

    return 0;
}
