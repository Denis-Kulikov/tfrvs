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

    void start(double epsilon) {
        int index = 0;
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
            calculate_V();
        } while (iterations < 10); // do {...} while(); - потому что iterations == 0 при запуске
    

    }

private:
    int first(int index) {
        for (int j = 0; j < J; ++j) 
            X[j] += C[index][j];

        auto min_value = std::min_element(X.begin(), X.end());
        auto index_min = std::distance(X.begin(), min_value);
        ++x[index_min];

        return second(index_min);
    }

    int second(int index) {
        for (int i = 0; i < I; ++i)
            Y[i] += C[i][index];

        auto max_value = std::max_element(Y.begin(), Y.end());
        auto index_max = std::distance(Y.begin(), max_value);
        ++y[index_max];

        return index_max;
    }

    void calculate_V() {
        auto x_ = get_x();
        auto y_ = get_y();
        V_prev = V;
        double v = 0;

        for (int i = 0; i < I; ++i) {
            for (int j = 0; j < J; ++j) {
                v += C[i][j] * x_[j] * y_[i];
            }
        }

        V = v;
    }

    const vector<vector<double>>& C; 
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

    vector<vector<double>> C;
    vector<double> i1, i2, i3;
    i1 = {6, 9, 0, 2};
    i2 = {0, 6, 3, 1};
    i3 = {3, 1, 2, 4};

    C.push_back(i3);
    C.push_back(i2);
    C.push_back(i1);

    print_matrix(C);

    brown_method bm(C);
    bm.start(1);

    cout << "V ≈ " << bm.get_V() << endl;
    cout << "iterations = " << bm.get_iterations() << endl;
    cout << "x = ";
    for (const auto& it : bm.get_x())
        cout << it << " ";
    cout << endl;
    cout << "y = ";
    for (const auto& it : bm.get_y())
        cout << it << " ";
    cout << endl;

    return 0;
}
