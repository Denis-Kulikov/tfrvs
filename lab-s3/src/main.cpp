#include <iostream>
#include <fstream>
#include <vector>
#include "include/math.hpp"
#include <sstream>
#include <string>

int main() {
    std::ofstream file("../data/table.csv");
    if (!file.is_open()) {
        std::cerr << "Error: couldn't open the file '../data/table.csv'" << std::endl;
        return -1;
    }
    file << "lambda;mu;m;theta;T\n";

    int N = 65536;
    std::vector<double> lambdas = {1e-6, 1e-7, 1e-5};
    std::vector<double> mus = {1, 10, 100, 1000};
    std::vector<int> ms = {1, 2, 3};
    std::vector<int> ns;
    for (int i = 65527; i <= 65536; ++i) ns.push_back(i);

    for (double lambda : lambdas) {
        std::cout << "lambda: " << lambda << std::endl;
        for (double mu : mus) {
            std::cout << "\tmu: " << mu << std::endl;
            for (int m : ms) { 
                std::cout << "\t\tm: " << m << std::endl;
                file << lambda << ";" << mu << ";" << m << ";";

                std::ostringstream thetaStream;
                for (int n : ns) {
                    thetaStream << computeTheta(N, m, n, lambda, mu) << ",";
                }
                std::string thetaStr = thetaStream.str();
                thetaStr.pop_back();
                file << thetaStr << ";";

                std::ostringstream TStream;
                for (int n : ns) {
                    TStream << computeT(N, m, n, lambda, mu) << ",";
                }
                std::string TStr = TStream.str();
                TStr.pop_back();
                file << TStr << "\n";
            }
        }
        std::cout << std::endl;
    }
    
    file.close();

    return 0;
}
