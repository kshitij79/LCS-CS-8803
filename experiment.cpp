#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <random>
# include "dpll.h"

// The DPLL algorithm and simplify function go here...

std::vector<std::vector<int>> generateRandom3SAT(int N, int L) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> varDist(1, N);
    std::uniform_real_distribution<> signDist(0, 1);
    
    std::vector<std::vector<int>> formula;
    for (int i = 0; i < L; ++i) {
        std::vector<int> clause;
        while (clause.size() < 3) {
            int var = varDist(gen);
            if (std::find(clause.begin(), clause.end(), var) == clause.end() &&
                std::find(clause.begin(), clause.end(), -var) == clause.end()) {
                int sign = signDist(gen) < 0.5 ? -1 : 1;
                clause.push_back(sign * var);
            }
        }
        formula.push_back(clause);
    }
    return formula;
}

int numSplittingApplications = 0; // A global counter for the splitting applications

bool DPLLWrapper(std::vector<std::vector<int>>& clauses) {
    std::unordered_map<int, bool> assignment;
    auto start = std::chrono::high_resolution_clock::now();
    
    bool result = randomHeuristicDPLL(clauses, assignment);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time taken: " << diff.count() << " s" << std::endl;
    std::cout << "Number of splitting-rule applications: " << numSplittingApplications << std::endl;
    
    return result;
}

int main() {
    // std::vector<double> ratios = {3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0};
    std::vector<double> ratios = {3.2};

    int N = 65; // Adjust as needed
    int runs = 1;
    
    for (double ratio : ratios) {
        int L = static_cast<int>(ratio * N);
        int satisfiableCount = 0;
        
        for (int i = 0; i < runs; ++i) {
            std::vector<std::vector<int>> formula = generateRandom3SAT(N, L);
            bool isSatisfiable = DPLLWrapper(formula);
            if (isSatisfiable) {
                satisfiableCount++;
            }
        }
        
        std::cout << "For L/N ratio " << ratio << ", Satisfiable Probability: " << static_cast<double>(satisfiableCount) / runs << std::endl;
    }
    
    return 0;
}