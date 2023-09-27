// dpll.h
#include <vector>
#include <unordered_map>

#ifndef DPLL_H
#define DPLL_H

// Function declaration
bool simpleHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment, int & numSplittingApplications);

bool randomHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment, int & numSplittingApplications);

bool twoClauseHeuristicDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment, int & numSplittingApplications);

bool maxLiteralClauseHeuristicDPLL(std::vector<std::vector<int>>& clauses, std::unordered_map<int, bool>& assignment, int & numSplittingApplications);
#endif // DPLL_H
