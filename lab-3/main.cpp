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
        int index = 9;
        // int index = rand() % I;
        std::cout << "index: " << index << std::endl;

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
        return fabs(V - V_prev) <= epsilon;
    }

    void callback(int index) {
        do {
            ++iterations;
            index = first(index);
            // calculate_V(); // } while (iterations < 10);
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
    double epsilon;
    int I;
    int J;
    int iterations;
    brown_method* parent = nullptr;
};

int main() {
    srand(time(nullptr));
    double c1 = 1.0, c2 = 2.0, c3 = 3.0, epsilon = 0.01;
    int n = 11;

    vector<vector<double>> C = compute_payoff_matrix(n, c1, c2, c3);
    print_matrix(C);

    // vector<vector<double>> C;
    // vector<double> i1, i2, i3;
    // i3 = {3, 1, 2, 4};
    // i2 = {0, 6, 3, 1};
    // i1 = {6, 9, 0, 2};
    // C.push_back(i3);
    // C.push_back(i2);
    // C.push_back(i1);

    // i1 = {2, 0, 9, 6};
    // i2 = {1, 3, 6, 0};
    // i3 = {4, 2, 1, 3};
    // C.push_back(i1);
    // C.push_back(i2);
    // C.push_back(i3);
    // print_matrix(C);

    brown_method bm(C);
    bm.start(epsilon);
    auto results = *bm.results;

    double small = results[0].V;
    int small_index = 0;
    int iterations = 0;
    for (int i = 0; i < results.size(); ++i) {
        if (results[i].V < small) {
            small = results[i].V;
            small_index = i;
        }
        iterations += results[i].iterations;
    }
    cout << "Size = " << results.size() << endl;
    cout << "small V ≈ " << small << endl;
    cout << "iterations = " << iterations << endl;
    cout << "Oптимaльныe cмeшaнныe cтpaтeгии" << endl;
    cout << "ВЦ: = ";
    for (const auto& it : results[small_index].y)
        cout << it / results[small_index].iterations << " ";
    cout << endl;
    cout << "Диcпeтчep: = ";
    for (const auto& it : results[small_index].x)
        cout << it / results[small_index].iterations << " ";
    cout << endl;
    cout << endl;
    cout << endl;


    // for (int i = 0; i < results.size(); ++i) {
    //     cout << "V ≈ " << results[i].V << endl;
    //     cout << "Oптимaльныe cмeшaнныe cтpaтeгии" << endl;
    //     cout << "ВЦ: = ";
    //     for (const auto& it : results[i].y)
    //         cout << it << " ";
    //     cout << endl;
    //     cout << "Диcпeтчep: = ";
    //     for (const auto& it : results[i].x)
    //         cout << it << " ";
    //     cout << endl;
    //     cout << endl;
    // }


    // double max_value = *std::max_element(bm.results->begin(), bm.results->end());
    
    // cout << "V ≈ " << results.size() << endl;
    // cout << "iterations = " << bm.get_iterations() << endl;
    // cout << "Oптимaльныe cмeшaнныe cтpaтeгии" << endl;
    // cout << "ВЦ: = ";
    // for (const auto& it : bm.get_y())
    //     cout << it << " ";
    // cout << endl;
    // cout << "Диcпeтчep: = ";
    // for (const auto& it : bm.get_x())
    //     cout << it << " ";
    // cout << endl;

    return 0;
}
