#include "dpll.h"
#include<bits/stdc++.h>

using namespace std;

int twoClauseHeuristic(const std::vector<std::vector<int>>& clauses, const std::unordered_map<int, bool>& assignment) {
    std::unordered_map<int, int> literalCounts;
    for(const auto& clause : clauses) {
        if(clause.size() == 2) {
            for(int lit : clause) {
                if(assignment.find(abs(lit)) == assignment.end()) {
                    literalCounts[lit]++;
                }
            }
        }
    }

    if(literalCounts.empty()) return std::abs(clauses[0][0]);

    int maxCount = 0;
    std::vector<int> maxLiterals;
    for(const auto& [literal, count] : literalCounts) {
        if(count > maxCount) {
            maxCount = count;
            maxLiterals = {literal};
        } else if(count == maxCount) {
            maxLiterals.push_back(literal);
        }
    }

    // Randomly select among the tied literals
    std::random_device rd;
    std::mt19937 g(rd());
    return maxLiterals[g() % maxLiterals.size()];
}

int maxLiteralHeuristic(const std::vector<std::vector<int>>& clauses, const std::unordered_map<int, bool>& assignment) {
    std::unordered_map<int, int> literalCounts;
    for(const auto& clause : clauses) {
        for(int lit : clause) {
            if(assignment.find(abs(lit)) == assignment.end()) {
                literalCounts[lit]++;
            }
        }
    }

    if(literalCounts.empty()) return std::abs(clauses[0][0]);

    int maxCount = 0;
    std::vector<int> maxLiterals;
    for(const auto& [literal, count] : literalCounts) {
        if(count > maxCount) {
            maxCount = count;
            maxLiterals = {literal};
        } else if(count == maxCount) {
            maxLiterals.push_back(literal);
        }
    }

    // Randomly select among the tied literals
    std::random_device rd;
    std::mt19937 g(rd());
    return maxLiterals[g() % maxLiterals.size()];
}

int mixedHeuristic(const std::vector<std::vector<int>>& clauses, const std::unordered_map<int, bool>& assignment) {
    int literal = twoClauseHeuristic(clauses, assignment);
    if (literal != std::abs(clauses[0][0])) return literal;  // This means there was a 2-clause literal selected.
    return maxLiteralHeuristic(clauses, assignment);
}

std::pair<std::vector<int>, bool> unit_clauses(const std::vector<std::vector<int>>& clauses) {
    std::vector<int> single_clauses;
    std::unordered_set<int> units_set;
    for (const auto& cl : clauses) {
        if (cl.size() == 1) {
            single_clauses.push_back(cl[0]);
            if (units_set.find(-cl[0]) != units_set.end()) { 
                return {{}, false}; // return empty vector and unsatisfiable flag
            }
            units_set.insert(cl[0]);
        }
    }
    return {single_clauses, true}; // return the unit clauses and satisfiable flag
}

std::vector<std::vector<int>> unit_propagation(const std::vector<std::vector<int>>& clauses, const std::unordered_set<int>& UC) {
    std::vector<std::vector<int>> C;
    for (const auto& cl : clauses) {
        std::vector<int> C_;
        bool flag = false;
        for (int li : cl) {
            if (UC.find(li) != UC.end()) {
                flag = true;
                break;
            }
            if (UC.find(-li) != UC.end()) {
                continue;
            }
            C_.push_back(li);
        }
        if (!flag) {
            C.push_back(C_);
        }
    }
    return C;
}

bool simpleHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment, int & numSplittingApplications) {
    if (clauses.empty()) return true;
    if (std::any_of(clauses.begin(), clauses.end(), [](const std::vector<int>& cl) { return cl.empty(); })) return false;

    // Unit Propagation
    auto unit_clauses_result = unit_clauses(clauses);
    std::vector<int> unit_clauses_list = unit_clauses_result.first;

    if (!unit_clauses_result.second) {
        return false; // Formula is unsatisfiable
    }

    // std::vector<int> unit_clauses_list = unit_clauses(clauses);
    for (int unit : unit_clauses_list) {
        if (std::find(unit_clauses_list.begin(), unit_clauses_list.end(), -unit) != unit_clauses_list.end()) {
            return false;
        }
    }
    if (!unit_clauses_list.empty()) {
        std::unordered_set<int> UC(unit_clauses_list.begin(), unit_clauses_list.end());
        std::vector<std::vector<int>> new_list = unit_propagation(clauses, UC);
        for (int unit : unit_clauses_list) {
            assignment[std::abs(unit)] = unit > 0;
        }
        return simpleHeuristicDPLL(new_list, assignment, numSplittingApplications);
    }

    // Take out a random literal from clauses
    int literal = std::abs(clauses[0][0]);
    numSplittingApplications++; 

    std::unordered_map<int, bool> assignment1 = assignment;

    // Set the first literal to true
    assignment1[literal] = true;
    std::vector<std::vector<int>> new_list = unit_propagation(clauses, {literal});
    if (simpleHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    // If the first does not satisfy, set the literal to false
    assignment1[literal] = false;
    new_list = unit_propagation(clauses, {-literal});
    if (simpleHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    return false;
}

bool randomHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment, int & numSplittingApplications) {
    if (clauses.empty()) return true;
    if (std::any_of(clauses.begin(), clauses.end(), [](const std::vector<int>& cl) { return cl.empty(); })) return false;

    // Unit Propagation
    auto unit_clauses_result = unit_clauses(clauses);
    std::vector<int> unit_clauses_list = unit_clauses_result.first;

    if (!unit_clauses_result.second) {
        return false; // Formula is unsatisfiable
    }

    // std::vector<int> unit_clauses_list = unit_clauses(clauses);
    for (int unit : unit_clauses_list) {
        if (std::find(unit_clauses_list.begin(), unit_clauses_list.end(), -unit) != unit_clauses_list.end()) {
            return false;
        }
    }
    if (!unit_clauses_list.empty()) {
        std::unordered_set<int> UC(unit_clauses_list.begin(), unit_clauses_list.end());
        std::vector<std::vector<int>> new_list = unit_propagation(clauses, UC);
        for (int unit : unit_clauses_list) {
            assignment[std::abs(unit)] = unit > 0;
        }
        return randomHeuristicDPLL(new_list, assignment, numSplittingApplications);
    }

    // Take out a random literal from clauses
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, clauses.size() - 1);
    int row = dis(gen);
    std::uniform_int_distribution<> dis2(0, clauses[row].size() - 1);
    int column = dis2(gen);
    int literal = std::abs(clauses[row][column]);
    numSplittingApplications++; 

    std::unordered_map<int, bool> assignment1 = assignment;

    // Set the first literal to true
    assignment1[literal] = true;
    std::vector<std::vector<int>> new_list = unit_propagation(clauses, {literal});
    if (randomHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    // If the first does not satisfy, set the literal to false
    assignment1[literal] = false;
    new_list = unit_propagation(clauses, {-literal});
    if (randomHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    return false;
}


bool twoClauseHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment, int & numSplittingApplications) {
    if (clauses.empty()) return true;
    if (std::any_of(clauses.begin(), clauses.end(), [](const std::vector<int>& cl) { return cl.empty(); })) return false;

    // Unit Propagation
    auto unit_clauses_result = unit_clauses(clauses);
    std::vector<int> unit_clauses_list = unit_clauses_result.first;

    if (!unit_clauses_result.second) {
        return false; // Formula is unsatisfiable
    }

    // std::vector<int> unit_clauses_list = unit_clauses(clauses);
    for (int unit : unit_clauses_list) {
        if (std::find(unit_clauses_list.begin(), unit_clauses_list.end(), -unit) != unit_clauses_list.end()) {
            return false;
        }
    }
    if (!unit_clauses_list.empty()) {
        std::unordered_set<int> UC(unit_clauses_list.begin(), unit_clauses_list.end());
        std::vector<std::vector<int>> new_list = unit_propagation(clauses, UC);
        for (int unit : unit_clauses_list) {
            assignment[std::abs(unit)] = unit > 0;
        }
        return twoClauseHeuristicDPLL(new_list, assignment, numSplittingApplications);
    }

    // Follow two clause heuristic
    int literal = twoClauseHeuristic(clauses, assignment);
    numSplittingApplications++; 

    std::unordered_map<int, bool> assignment1 = assignment;

    // Set the first literal to true
    assignment1[literal] = true;
    std::vector<std::vector<int>> new_list = unit_propagation(clauses, {literal});
    if (twoClauseHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    // If the first does not satisfy, set the literal to false
    assignment1[literal] = false;
    new_list = unit_propagation(clauses, {-literal});
    if (twoClauseHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    return false;
}

bool maxLiteralClauseHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment, int & numSplittingApplications) {
    if (clauses.empty()) return true;
    if (std::any_of(clauses.begin(), clauses.end(), [](const std::vector<int>& cl) { return cl.empty(); })) return false;

    // Unit Propagation
    auto unit_clauses_result = unit_clauses(clauses);
    std::vector<int> unit_clauses_list = unit_clauses_result.first;

    if (!unit_clauses_result.second) {
        return false; // Formula is unsatisfiable
    }

    // std::vector<int> unit_clauses_list = unit_clauses(clauses);
    for (int unit : unit_clauses_list) {
        if (std::find(unit_clauses_list.begin(), unit_clauses_list.end(), -unit) != unit_clauses_list.end()) {
            return false;
        }
    }
    if (!unit_clauses_list.empty()) {
        std::unordered_set<int> UC(unit_clauses_list.begin(), unit_clauses_list.end());
        std::vector<std::vector<int>> new_list = unit_propagation(clauses, UC);
        for (int unit : unit_clauses_list) {
            assignment[std::abs(unit)] = unit > 0;
        }
        return maxLiteralClauseHeuristicDPLL(new_list, assignment, numSplittingApplications);
    }

    // Follow two clause heuristic
    int literal = mixedHeuristic(clauses, assignment);
    numSplittingApplications++; 

    std::unordered_map<int, bool> assignment1 = assignment;

    // Set the first literal to true
    assignment1[literal] = true;
    std::vector<std::vector<int>> new_list = unit_propagation(clauses, {literal});
    if (maxLiteralClauseHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    // If the first does not satisfy, set the literal to false
    assignment1[literal] = false;
    new_list = unit_propagation(clauses, {-literal});
    if (maxLiteralClauseHeuristicDPLL(new_list, assignment1, numSplittingApplications)) {
        assignment = assignment1;
        return true;
    }

    return false;
}
