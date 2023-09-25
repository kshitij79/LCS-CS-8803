// dpll.h
#include <vector>
#include <unordered_map>

#ifndef DPLL_H
#define DPLL_H

// Function declaration
bool simpleHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment);

bool randomHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment);

bool twoClauseHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment);

bool VSIDSHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment);

#endif // DPLL_H