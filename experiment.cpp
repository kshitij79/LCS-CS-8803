#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <random>
# include "dpll.h"
#include <future>
#include <chrono>
#include <thread>
#include <fstream> // Added for file operations

template<typename Func, typename... Args>
auto runWithTimeout(Func&& func, int timeoutInSeconds, Args&&... args) -> std::invoke_result_t<Func, Args...>
{
    // Run the function in a separate thread and get the future
    auto future = std::async(std::launch::async, std::forward<Func>(func), std::forward<Args>(args)...);

    // Wait for the result or a timeout
    if(future.wait_for(std::chrono::seconds(timeoutInSeconds)) == std::future_status::timeout) {
        throw std::runtime_error("Timeout reached!");
    }

    // Retrieve the result
    return future.get();
}

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

bool DPLLWrapper(std::vector<std::vector<int>>& clauses, int & numSplittingApplications, std::chrono::duration<double> & time, std::string procedure) {
    std::unordered_map<int, bool> assignment;
    auto start = std::chrono::high_resolution_clock::now();
    
    bool result;

    if(procedure == "Benchmark Heuristic")
        result = benchmarkHeuristicDPLL(clauses, assignment, numSplittingApplications);
    else if(procedure == "Random Heuristic")
        result = randomHeuristicDPLL(clauses, assignment, numSplittingApplications);
    else if(procedure == "Naive Heuristic")
        result = simpleHeuristicDPLL(clauses, assignment, numSplittingApplications);    
    else
        result = twoClauseHeuristicDPLL(clauses, assignment, numSplittingApplications);   

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    time = diff;
    
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    return result;
}

int main() {

    std::ofstream outputFile("results.csv"); // Create a CSV file for output

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    outputFile << "Procedure,Ratio,MedianTime,MedianSplits,NoTimeout,SatisfiabilityRatio\n";

    std::vector<double> ratios = {3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0};

    int N = 150; // Adjust as needed
    int runs = 100;
    std::vector<std::string> procedures = {"Benchmark Heuristic", "Two-Clause Heuristic"};

    for (double ratio : ratios) {
        int L = static_cast<int>(ratio * N);
        std::vector<std::vector<std::vector<int>>> formula;
        for (std::string procedure: procedures) {
            int satisfiableCount = 0;
            int noTimeoutCount = 0;
            std::vector<int> splitsData;           // For storing splits for each run.
            std::vector<double> timeData;          // For storing time for each run.
            
            for(int i = 0; i < runs; ++i){
                if(formula.size()<=i)
                    formula.push_back(generateRandom3SAT(N, L));
                
                int noSplits = 0;
                std::chrono::duration<double> time(0.0);
                try {
                    bool isSatisfiable = runWithTimeout([&]() {
                                            return DPLLWrapper(formula[i], noSplits, time, procedure);
                                        }, 10);
                    if (isSatisfiable) {
                        satisfiableCount++;
                    }
                    splitsData.push_back(noSplits);
                    timeData.push_back(time.count());
                } catch (const std::exception& e) {
                    splitsData.push_back(10000);
                    timeData.push_back(10000);
                    noTimeoutCount++; 
                    std::cout<< procedure <<" timeout " << i << " " << ratio <<std::endl;
                }
            }
            
            std::sort(splitsData.begin(), splitsData.end());
            std::sort(timeData.begin(), timeData.end());
            // Compute medians
            double medianSplits = (runs % 2 == 0) ? (splitsData[runs/2 - 1] + splitsData[runs/2]) / 2.0
                                                : splitsData[runs/2];

            double medianTime = (runs % 2 == 0) ? (timeData[runs/2 - 1] + timeData[runs/2]) / 2.0
                                            : timeData[runs/2];

            outputFile << procedure << "," << ratio << "," << medianTime << "," << medianSplits << "," << noTimeoutCount << ","
                       << static_cast<double>(satisfiableCount) / static_cast<double>(runs) << "\n";
            // std::cout << "Procedure Name:" << procedure << std::endl;
            // std::cout << "Time taken: " << medianTime << " s" << std::endl;
            // std::cout << "Number of splitting-rule applications: " << medianSplits << std::endl;
            // std::cout << "For L/N ratio " << ratio << ", Satisfiable Probability: "<< satisfiableCount<<" " << static_cast<double>(satisfiableCount) / static_cast<double>(runs) << std::endl;
            // std::cout << "Procedure Ends" << std::endl;
        }
        
    }
    
    outputFile.close();

    return 0;
}
