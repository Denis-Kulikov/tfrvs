#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

const double LAMBDA = 0.024; // Интенсивность отказов (1/ч)
const double MU = 0.71;      // Интенсивность восстановления (1/ч)
const int M = 1;             // Число восстанавливающих устройств
const int T = 24;            // max t

double factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

double poissonProbability(int k, double lambda_t) {
    return (pow(lambda_t, k) / factorial(k)) * exp(-lambda_t);
}

// Функция оперативной надёжности R*(t)
double computeReliability(int N, int n, double t) {
    double Q = 0.0;
    for (int i = n; i <= N; i++) {
        double p_0 = 0.0;
        for (int l = 0; l <= N; l++) {
            int deltaX_1 = (N - i - M >= 0) ? 1 : 0;
            int deltaX_2 = (M - N + i >= 0) ? 1 : 0;
            double ul = (pow(MU * t, l) / factorial(l)) // (𝜇𝑡)^𝑙 / 𝑙! 
                        * (
                            (deltaX_1 * pow(M, l) * exp(-M * MU * t)) // Δ(𝑁 − 𝑖 − 𝑚) * 𝑚^𝑙 * 𝑒^−𝑖𝜇𝑡
                            + 
                            (deltaX_2 * pow(N - i, l) * exp(-(N - i) * MU * t)) // Δ(𝑚 − 𝑁 + 𝑖) * (𝑁 − 𝑖)^𝑙 * 𝑒^−(𝑁−𝑖)𝜇𝑡
                        );
            double pr = 0.0;
            for (int r = 0; r <= (i - n + l); r++) {
                pr += poissonProbability(r, i * LAMBDA * t); // (𝑖𝜆𝑡)^𝑟/𝑟! * 𝑒^−𝑖𝜆𝑡
            }
            p_0 += pow((MU / LAMBDA), l) / factorial(l); // P_0 = (𝜇/𝜆)^𝑙 / 𝑙!
            Q += ul * pr;
        }
        double Pi = pow((MU / LAMBDA), i) / factorial(i) * pow(p_0, -1); // (𝜇/𝜆)^𝑖 / 𝑖! * P_0^-1
        Q *= Pi;
    }
    return Q;
}

// Функция оперативной восстановимости U*(t)
double computeMaintainability(int N, int n, double t) {
    double Pi = 0.0;
    for (int i = 0; i <= n - 1; i++) {
        double pi_r = 0.0;
        for (int r = 0; r <= N; r++) {
            double ul = 0.0;
            for (int l = 0; l <= n - i - 1 + r; l++) {
                int deltaX_1 = (N - i - M >= 0) ? 1 : 0;
                int deltaX_2 = (M - N + i >= 0) ? 1 : 0;
                ul += (pow(MU * t, l) / factorial(l)) * (deltaX_1 * pow(M, l) * exp(-M * MU * t)) +
                      (deltaX_2 * pow(N - i, l) * exp(-(N - i) * MU * t));
            }
            pi_r += poissonProbability(r, i * LAMBDA * t) * ul;
        }
        double p_0 = 0.0;
        for (int j = 0; j <= N; j++) {
            p_0 += pow((MU / LAMBDA), j) / factorial(j);
        }
        Pi += pow((MU / LAMBDA), i) / factorial(i) * pow(p_0, -1) * pi_r;
    }
    return 1.0 - Pi;
}

// Коэффициент готовности S
double computeAvailability(int N, int n, int m) {
    double sum = 0.0;
    double alpha = m * MU / LAMBDA; // m * μ / λ
    for (int j = 0; j <= n - 1; j++) {
        sum += pow(alpha, j) / factorial(j);
    }
    return 1 - sum * exp(-alpha);
}


void r_star() {
    const int N = 10;
    std::ofstream output("../data/r_star.txt");
    for (int n = 8; n <= N; n++) {
        for (int t = 0; t <= T; t += 2) {
            output << computeReliability(N, n, t) << "\t";
        }
        output << "\n";
    }
    output.close();
}

void u_star() {
    const int N = 16;
    std::ofstream output("../data/u_star.txt");
    for (int n = 10; n <= N; n++) {
        for (int t = 0; t <= T; t += 2) {
            output << computeMaintainability(N, n, t) << "\t";
        }
        output << "\n";
    }
    output.close();
}

void s() {
    const int N = 16;
    std::ofstream output("../data/s.txt");
    for (int m = 1; m <= 16; m <<= 4) {
        for (int n = 11; n <= N; n++) {
            output << computeAvailability(N, n, m) << "\t";
        }
        output << "\n";
    }

    output.close();
}

int main() {
    r_star();
    u_star();
    s();

    return 0;
}
