#include <iostream>
#include <fstream>
#include <vector>
#include "include/math.hpp"
#include "include/parser.hpp"

using namespace std;

void writeThetaData(const string& filename, vector<vector<double>>& result) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error открытия файла: " << filename << std::endl;
        return;
    }

    for (const auto& i: result) {
        for (const auto& j: i)
            file << j << " ";
        file << '\n';
    }
    
    file.close();
}

class Task1 {
public:
    Task1() {
        if (!readParams("task1.txt", N, m, lambda, mu, n_values))
            cerr << "Ошибка чтения параметров" << endl;
    }

    void theta_1(vector<vector<double>>& result) {
        vector<double> mu_values = {1, 10, 100, 1000};
        clear(result, mu_values.size());

        for (const auto mu: mu_values) {
            vector<double> theta;
            theta.reserve(n_values.size());

            for (const auto& n: n_values)
                theta.emplace_back(computeTheta(N, m, n, lambda, mu));

            result.emplace_back(theta); 
        }
    }

    void theta_2(vector<vector<double>>& result) {
        vector<double> lambda_values = {1e-5, 1e-6,  1e-7, 1e-8, 1e-9};
        clear(result, lambda_values.size());

        for (const auto lambda: lambda_values) {
            vector<double> theta;
            theta.reserve(n_values.size());
            
            for (const auto& n: n_values)
                theta.emplace_back(computeTheta(N, m, n, lambda, mu));

            result.emplace_back(theta); 
        }
    }

    void theta_3(vector<vector<double>>& result) {
        vector<int> m_values = {1, 2, 3, 4};
        clear(result, m_values.size());

        for (const auto m: m_values) {
            vector<double> theta;
            theta.reserve(n_values.size());
            
            for (const auto& n: n_values)
                theta.emplace_back(computeTheta(N, m, n, lambda, mu));

            result.emplace_back(theta); 
        }
    }

protected:
    void clear(vector<vector<double>>& result, size_t size) {
        result.clear();
        result.reserve(size);
    }

    int N;
    int m;
    double lambda;
    double mu;
    vector<int> n_values;
};

class Task2 {
public:
    static void T1(vector<vector<double>>& result) {
        int N = 1000;
        double lambda = 1e-3;
        int m = 1;
        vector<int> n_values = {900, 910, 920, 930, 940, 950, 960, 970, 980, 990, 1000};
        vector<double> mu_values = {1, 2, 4, 6};

        clear(result, mu_values.size());

        for (const auto mu: mu_values) {
            vector<double> t;
            t.reserve(n_values.size());

            for (const auto& n: n_values)
                t.emplace_back(computeT(N, m, n, lambda, mu));

            result.emplace_back(t); 
        }
    }

    static void T2(vector<vector<double>>& result) {
        int N = 8192;
        double mu = 1;
        int m = 1;
        vector<double> lambda_values = {1e-5, 1e-6,  1e-7, 1e-8, 1e-9};
        vector<int> n_values;
        n_values.reserve((8192 - 8092 ) / 10 + 1);
        for (int i = 8092; i <= 8192; i += 10)
            n_values.emplace_back(i);

        clear(result, lambda_values.size());

        for (const auto lambda: lambda_values) {
            vector<double> t;
            t.reserve(n_values.size());
            
            for (const auto& n: n_values)
                t.emplace_back(computeT(N, m, n, lambda, mu));

            result.emplace_back(t); 
        }
    }

    static void T3(vector<vector<double>>& result) {
        int N = 8192;
        double mu = 1;
        double lambda = 1e-5;
        int m = 1;
        vector<int> m_values = {1, 2, 3, 4};
        vector<int> n_values;
        n_values.reserve((8192 - 8092 ) / 10 + 1);
        for (int i = 8092; i <= 8192; i += 10)
            n_values.emplace_back(i);

        clear(result, m_values.size());

        for (const auto m: m_values) {
            vector<double> t;
            t.reserve(n_values.size());
            
            for (const auto& n: n_values)
                t.emplace_back(computeT(N, m, n, lambda, mu));

            result.emplace_back(t); 
        }
    }

protected:
    static void clear(vector<vector<double>>& result, size_t size) {
        result.clear();
        result.reserve(size);
    }
};

int main() {
    Task1 task1;
    vector<vector<double>> result;

    task1.theta_1(result);
    writeThetaData("../data/theta_mu.txt", result);
    task1.theta_2(result);
    writeThetaData("../data/theta_lambda.txt", result);
    task1.theta_3(result);
    writeThetaData("../data/theta_m.txt", result);

    Task2::T1(result);
    writeThetaData("../data/t_mu.txt", result);
    Task2::T2(result);
    writeThetaData("../data/t_lambda.txt", result);
    Task2::T3(result);
    writeThetaData("../data/t_m.txt", result);

    return 0;
}
