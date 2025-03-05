#include "include/math.hpp"
#include <iostream>

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
double computeT(int N, int m, int n, double lambda, double mu) {
    double T = 1.0 / MU(0);
    if (n == 1) return T;
    
    double product = 1.0;
    for (int l = 1; l < n; ++l)
        product *= (l * lambda) / MU(l);
    T *= product;


    for (int j = 1; j < n; ++j) {
        double product = 1.0;
        for (int l = j; l < n; ++l) 
            product *= l * lambda / MU(l);

        T += product / (j * lambda);
    }

    return T;
}
