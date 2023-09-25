#include "dpll.h"
#include<bits/stdc++.h>

using namespace std;

bool simplify(std::vector<std::vector<int>>& clauses, int literal) {
    bool isUnit = false;
    auto clause_it = clauses.begin();
    while(clause_it != clauses.end()) {
        if(std::find(clause_it->begin(), clause_it->end(), literal) != clause_it->end()) {
            // The clause is satisfied by the literal, so remove the entire clause
            clause_it = clauses.erase(clause_it);
        } else {
            auto lit_it = std::find(clause_it->begin(), clause_it->end(), -literal);
            if(lit_it != clause_it->end()) {
                // The clause contains the negated literal, so remove the literal
                clause_it->erase(lit_it);
                if(clause_it->empty()) return false; // Empty clause, so the formula is unsatisfiable
            }
            clause_it++;
        }
    }
    return true;
}

bool simpleHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment) {
    // If there are no clauses, it's satisfiable
    if(clauses.empty()) return true;
    
    // If there's an empty clause, it's unsatisfiable
    for(const auto& clause : clauses) if(clause.empty()) return false;

    // Find the first unassigned literal
    int literal = 0;
    for(const auto& clause : clauses) {
        for(int lit : clause) {
            if(assignment.find(abs(lit)) == assignment.end()) {
                literal = lit;
                break;
            }
        }
        if(literal != 0) break;
    }

    // If all literals are assigned, return true as every clause must be satisfied
    if(literal == 0) return true;

    // Try assigning the literal to true
    assignment[abs(literal)] = (literal > 0);
    auto tempClauses = clauses;
    if(simplify(tempClauses, literal)) {
        if(simpleHeuristicDPLL(tempClauses, assignment)) return true;
    }
    
    // Backtrack and try assigning the literal to false
    assignment[abs(literal)] = (literal < 0);
    tempClauses = clauses;
    if(simplify(tempClauses, -literal)) {
        if(simpleHeuristicDPLL(tempClauses, assignment)) return true;
    }

    // Neither assignment resulted in satisfiability
    assignment.erase(abs(literal));
    return false;
}

bool randomHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment) {
    // If there are no clauses, it's satisfiable
    if(clauses.empty()) return true;
    
    // If there's an empty clause, it's unsatisfiable
    for(const auto& clause : clauses) if(clause.empty()) return false;

    // Setup for random choice
    std::vector<int> unassigned_literals;
    for(const auto& clause : clauses) {
        for(int lit : clause) {
            if(assignment.find(abs(lit)) == assignment.end() && 
               std::find(unassigned_literals.begin(), unassigned_literals.end(), lit) == unassigned_literals.end()) {
                unassigned_literals.push_back(lit);
            }
        }
    }

    // If all literals are assigned, return true
    if(unassigned_literals.empty()) return true;

    // Randomly select an unassigned literal for splitting
    std::random_device rd;
    std::mt19937 g(rd());
    int random_index = g() % unassigned_literals.size();
    int literal = unassigned_literals[random_index];

    // Find the first unassigned literal
    for(const auto& clause : clauses) {
        for(int lit : clause) {
            if(assignment.find(abs(lit)) == assignment.end()) {
                literal = lit;
                break;
            }
        }
        if(literal != 0) break;
    }

    // If all literals are assigned, return true as every clause must be satisfied
    if(literal == 0) return true;

    // Try assigning the literal to true
    assignment[abs(literal)] = (literal > 0);
    auto tempClauses = clauses;
    if(simplify(tempClauses, literal)) {
        if(randomHeuristicDPLL(tempClauses, assignment)) return true;
    }
    
    // Backtrack and try assigning the literal to false
    assignment[abs(literal)] = (literal < 0);
    tempClauses = clauses;
    if(simplify(tempClauses, -literal)) {
        if(randomHeuristicDPLL(tempClauses, assignment)) return true;
    }

    // Neither assignment resulted in satisfiability
    assignment.erase(abs(literal));
    return false;
}

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

    if(literalCounts.empty()) return 0;

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

bool twoClauseHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment) {
    // If there are no clauses, it's satisfiable
    if(clauses.empty()) return true;
    
    // If there's an empty clause, it's unsatisfiable
    for(const auto& clause : clauses) if(clause.empty()) return false;

    // Find the first unassigned literal
    int literal = twoClauseHeuristic(clauses, assignment);
    for(const auto& clause : clauses) {
        for(int lit : clause) {
            if(assignment.find(abs(lit)) == assignment.end()) {
                literal = lit;
                break;
            }
        }
        if(literal != 0) break;
    }

    // If all literals are assigned, return true as every clause must be satisfied
    if(literal == 0) return true;

    // Try assigning the literal to true
    assignment[abs(literal)] = (literal > 0);
    auto tempClauses = clauses;
    if(simplify(tempClauses, literal)) {
        if(simpleHeuristicDPLL(tempClauses, assignment)) return true;
    }
    
    // Backtrack and try assigning the literal to false
    assignment[abs(literal)] = (literal < 0);
    tempClauses = clauses;
    if(simplify(tempClauses, -literal)) {
        if(simpleHeuristicDPLL(tempClauses, assignment)) return true;
    }

    // Neither assignment resulted in satisfiability
    assignment.erase(abs(literal));
    return false;
}

std::unordered_map<int, double> vsids_score;

// Initialize VSIDS scores
void initializeVSIDS(const std::vector<std::vector<int>>& clauses) {
    for (const auto& clause : clauses) {
        for (int lit : clause) {
            vsids_score[abs(lit)] = 1.0; // Initialize with a default score
        }
    }
}

// Decay VSIDS scores periodically
void decayVSIDS() {
    for (auto& [var, score] : vsids_score) {
        score *= 0.95; // Decay factor; adjust based on your preference
    }
}

// Increase the score of a variable after a conflict
void bumpVSIDS(int var) {
    vsids_score[var] += 1.0;
}

// Get variable with the highest VSIDS score that hasn't been assigned yet
int getHighestScoreLiteral(const std::vector<std::vector<int>>& clauses, const std::unordered_map<int, bool>& assignment) {
    int bestVar = 0;
    double bestScore = -1.0;
    
    for (const auto& clause : clauses) {
        for (int lit : clause) {
            int var = abs(lit);
            if (assignment.find(var) == assignment.end() && vsids_score[var] > bestScore) {
                bestScore = vsids_score[var];
                bestVar = lit;  // select the literal, not just the variable
            }
        }
    }
    return bestVar;  // Returns 0 if no unassigned variable is found
}

bool VSIDSHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment) {
    // If there are no clauses, it's satisfiable
    if(clauses.empty()) return true;
    
    // If there's an empty clause, it's unsatisfiable
    for(const auto& clause : clauses) if(clause.empty()) return false;

    // Find the first unassigned literal
    int literal = getHighestScoreLiteral(clauses, assignment);
    if (literal == 0) return true; // All literals are assigned

    for(const auto& clause : clauses) {
        for(int lit : clause) {
            if(assignment.find(abs(lit)) == assignment.end()) {
                literal = lit;
                break;
            }
        }
        if(literal != 0) break;
    }

    // If all literals are assigned, return true as every clause must be satisfied
    if(literal == 0) return true;

    // Try assigning the literal to true
    assignment[abs(literal)] = (literal > 0);
    auto tempClauses = clauses;
    if(simplify(tempClauses, literal)) {
        if(simpleHeuristicDPLL(tempClauses, assignment)) return true;
    }
    
    // Backtrack and try assigning the literal to false
    assignment[abs(literal)] = (literal < 0);
    tempClauses = clauses;
    if(simplify(tempClauses, -literal)) {
        if(simpleHeuristicDPLL(tempClauses, assignment)) return true;
    }

    // Neither assignment resulted in satisfiability
    assignment.erase(abs(literal));
    return false;
}
