#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
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
    int n = C.size();
    cout << "Matrix C:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
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

    void start(double epsilon) {
        int index = rand() % I;
        V = 0;
        V_prev = numeric_limits<double>::infinity();
        iterations = 0;
        X = vector<double>(J, 0);
        Y = vector<double>(I, 0);
        x = vector<double>(J, 0);
        y = vector<double>(I, 0);

        do {
            ++iterations;
            index = first(index);
            calculate_V();
        } while (fabs(V - V_prev) > epsilon); // do {...} while(); - потому что iterations == 0 при запуске
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

private:
    int first(int index) {
        for (int i = 0; i < I; ++i)
            Y[i] += C[index][i];

        auto min_value = std::min_element(Y.begin(), Y.end());
        auto index_min = std::distance(Y.begin(), min_value);
        ++y[index_min];

        return second(index_min);
    }

    int second(int index) {
        for (int j = 0; j < J; ++j) 
            X[j] += C[j][index];

        auto max_value = std::max_element(X.begin(), X.end());
        auto index_max = std::distance(X.begin(), max_value);
        ++x[index_max];

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
    double c1 = 1.0, c2 = 2.0, c3 = 3.0, epsilon = 0.0001;
    int n = 11;

    vector<vector<double>> C = compute_payoff_matrix(n, c1, c2, c3);
    print_matrix(C);

    brown_method bm(C);
    bm.start(epsilon);

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
