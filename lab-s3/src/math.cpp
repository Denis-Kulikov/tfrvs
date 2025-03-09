#include "include/math.hpp"
#include <iostream>

#include <thread>
#include <mutex>

#define MU(l) computeMuL(l, N, m, mu)

double computeMuL(int l, int N, int m, double mu) {
    if ((N - m) <= l && l <= N) return (N - l) * mu; // (N - m) <= l <= N
    if (0 <= l && l < (N - m)) return m * mu;        // 0 <= l <= (N - m)
    return 0.0;
}

double computeTheta(int N, int m, int n, double lambda, double mu) {
    double theta = 1.0 / (n * lambda);

    for (int j = n + 1; j <= N; ++j) {
        double product = 1.0;
        for (int l = n; l < j; ++l) 
            product *= MU(l) / (l * lambda);

        theta += product / (j * lambda);
    }

    return theta;
}

double calculateT(int N, double lambda, int m, double mu, int n1) {
    auto mu_l = [N, m, mu](int l) -> double {
        return (l >= N - m && l <= N) ? (N - l) * mu : m * mu;
    };

    if (n1 == 1) {
        return 1.0 / mu;
    }
    double product1 = 1.0;
    for (int l = 1; l <= n1 - 1; l++) {
        product1 *= (l * lambda) / mu_l(l);
    }
    product1 /= mu;
    double sum = 0.0;
    for (int j = 1; j < n1 - 1; j++) {
        double product2 = 1.0;
        for (int l = j; l <= n1 - 1; l++) {
            product2 *= (l * lambda) / mu_l(l);
        }
        sum += product2 / (j * lambda);
    }
    return product1 + sum;
}

const int NUM_THREADS = 16;
std::mutex sum_mutex;

double computeT(int N, int m, int n, double lambda, double mu) {
    double T = 1.0 / MU(0, N, m, mu);
    if (n == 1) return T;
    
    double product = 1.0;
    auto computeProductBatch = [&](int start, int end, double &local_product) {
        local_product = 1.0;
        for (int l = start; l < end && l < n; ++l) {
            local_product *= (l * lambda) / MU(l, N, m, mu);
        }
    };
    
    std::vector<std::thread> product_threads;
    std::vector<double> local_products(NUM_THREADS, 1.0);
    int batch_size = (n - 1) / NUM_THREADS;
    int remainder = (n - 1) % NUM_THREADS;
    int start = 1;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int end = start + batch_size + (i < remainder ? 1 : 0);
        product_threads.emplace_back(computeProductBatch, start, end, std::ref(local_products[i]));
        start = end;
    }
    
    for (auto &t : product_threads) t.join();
    for (const auto &p : local_products) product *= p;
    T *= product;
    
    double sum = 0.0;
    auto computeBatch = [&](int start, int end) {
        double local_sum = 0.0;
        for (int j = start; j < end && j < n; ++j) {
            double product = 1.0;
            for (int l = j; l < n; ++l) 
                product *= (l * lambda) / MU(l, N, m, mu);
            local_sum += product / (j * lambda);
        }
        std::lock_guard<std::mutex> lock(sum_mutex);
        sum += local_sum;
    };
    
    std::vector<std::thread> sum_threads;
    start = 1;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int end = start + batch_size + (i < remainder ? 1 : 0);
        sum_threads.emplace_back(computeBatch, start, end);
        start = end;
    }
    
    for (auto &t : sum_threads) t.join();
    
    return T + sum;
}
