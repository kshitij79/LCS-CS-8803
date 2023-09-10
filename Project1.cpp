#include <bits/stdc++.h>

using namespace std;

int getID(int i, int j, int k) {
    return  k + 5 * (j - 1) + (i-1)*25;
}

tuple<int, int, int> getValues(int id) {
    int i = id / 25 + 1;
    int j = id % 25 / 5 + 1;
    int k = (id % 5!=0)?id%5:5;

    return make_tuple(i, j, k);
}

vector<vector<int>> clauseHouseDifferentCategory(int category_index) {
    vector<vector<int>> clauses;

    // Each house must have at least one category
    for (int i = 1; i <= 5; ++i) {
        vector<int> clause;
        for (int k = 1; k <= 5; ++k) {
            clause.push_back(getID(i, 1, k));
        }
        clauses.push_back(clause);
    }

    // Each house can have at most one category
    for (int i = 1; i <= 5; ++i) {
        for (int k1 = 1; k1 <= 5; ++k1) {
            for (int k2 = k1 + 1; k2 <= 5; ++k2) {
                vector<int> clause;
                clause.push_back(-getID(i, 1, k1));
                clause.push_back(-getID(i, 1, k2));
                clauses.push_back(clause);
            }
        }
    }

    // No two houses have the same category
    for (int k = 1; k <= 5; ++k) {
        for (int i1 = 1; i1 <= 5; ++i1) {
            for (int i2 = i1 + 1; i2 <= 5; ++i2) {
                vector<int> clause;
                clause.push_back(-getID(i1, 1, k));
                clause.push_back(-getID(i2, 1, k));
                clauses.push_back(clause);
            }
        }
    }

    return clauses;
}

vector<vector<int>> createImplicationClause(int category1Index, int category1Value, int category2Index, int category2Value) {

    vector <vector<int>> clauses;

    for (int i = 1; i <= 5; ++i) {
        
        int category1Id = getID(i, category1Index, category1Value); 
        int category2Id = getID(i, category2Index, category2Value);     
        vector<int> clause1 = {-category1Id, category2Id};  // ~ A V B
        clauses.push_back(clause1);

        vector<int> clause2 = {-category2Id, category1Id};  // ~ B V A
        clauses.push_back(clause1);
    }

    return clauses;
}

vector<vector<int>> createWhiteLeftNeighGreenClauses(int category1Index, int category1Value, int category2Index, int category2Value) {
    vector<vector<int>> clauses;
    // Looping through each of the 5 houses, starting from the second house
    for (int i = 2; i <= 5; ++i) {
        
        int greenId = getID(i - 1, category1Index, category1Value); 
        int whiteId = getID(i, category2Index, category2Value);  

        // Clause is "The green house is on the left of the white house."
        vector<int> clause1 = {-whiteId, greenId}; // ¬White ∨ Green (of the house to its left)
        clauses.push_back(clause1);
    }
    return clauses;
}

vector<vector<int>> createSingleTruthClauses(int houseIndex, int categoryIndex, int categoryValue) {
    vector<vector<int>> clauses;
    int categoryId = getID(houseIndex, categoryIndex, categoryValue); 

    // Single literal clause
    vector<int> clause = {categoryId};
    clauses.push_back(clause);
    return clauses;
}

vector<vector<int>> createHorseNeighDunhillClauses( int category1Index, int category1Value, int category2Index, int category2Value) {
    vector<vector<int>> clauses;
   for (int i = 1; i <= 5; ++i) {
        int horseId = getID(i, category1Index, category1Value);

        if (i > 1) {
            int dunhillLeft = getID(i - 1, category2Index, category2Value); 
            clauses.push_back({-horseId, dunhillLeft});  
        }
        
        if (i < 5) {
            int dunhillRight = getID(i + 1, category2Index, category2Value);  
            clauses.push_back({-horseId, dunhillRight});  
        }
    }
    return clauses;
}

vector<vector<int>> createClauses() {
    vector<vector<int>> clauses;
    for( int j=1; j<=5; j++){
        vector<vector<int>> houseDifferentColour = clauseHouseDifferentCategory(j);
        clauses.insert(clauses.end(), houseDifferentColour.begin(), houseDifferentColour.end());
    }

    vector<vector<int>> condition1Clause = createImplicationClause(2, 1, 1, 1); // National: 2, Brit: 1, Color: 1, Red: 1
    clauses.insert(clauses.end(), condition1Clause.begin(), condition1Clause.end());

    vector<vector<int>> condition2Clause = createImplicationClause(2, 2, 5, 2); // National: 2, Swede: 2, Pet: 5, Dogs: 2
    clauses.insert(clauses.end(), condition2Clause.begin(), condition2Clause.end());

    vector<vector<int>> condition3Clause = createImplicationClause(2, 3, 3, 1); // National: 2, Dane: 3, Bev: 3, Tea: 1
    clauses.insert(clauses.end(), condition3Clause.begin(), condition3Clause.end());
    
    vector<vector<int>> condition4Clause = createWhiteLeftNeighGreenClauses(1, 2, 1, 5); // Color: 1, Green: 2, COlor: 1, White: 5
    clauses.insert(clauses.end(), condition3Clause.begin(), condition3Clause.end());

    vector<vector<int>> condition5Clause = createImplicationClause(1, 2, 3, 3); // Color: 1, Green: 2, Beverage: 3, Coffee: 3
    clauses.insert(clauses.end(), condition5Clause.begin(), condition5Clause.end());

    vector<vector<int>> condition6Clause = createImplicationClause(4, 1, 5, 1); // Cigar: 4, Pall Mall: 1, Pet: 5, Birds: 1
    clauses.insert(clauses.end(), condition6Clause.begin(), condition6Clause.end());
    
    vector<vector<int>> condition7Clause = createImplicationClause(1, 4, 4, 2); // // Color: 1, Yellow: 4, Cigar: 4, Dunhill: 2
    clauses.insert(clauses.end(), condition7Clause.begin(), condition7Clause.end());

    vector<vector<int>> condition8Clause = createSingleTruthClauses(3, 3, 2); // House: 3, Bev: 3, MIlk: 2
    clauses.insert(clauses.end(), condition8Clause.begin(), condition8Clause.end());

    vector<vector<int>> condition9Clause = createSingleTruthClauses(1, 2, 5); // House: 1, Nationality: 2, Norwegian: 5
    clauses.insert(clauses.end(), condition9Clause.begin(), condition9Clause.end());

    vector<vector<int>> condition10Clause = createImplicationClause(4, 3, 5, 3); // Cigar: 4, Blends: 3, Pet: 5, Cats: 3
    clauses.insert(clauses.end(), condition10Clause.begin(), condition10Clause.end());

    vector<vector<int>> condition11Clause = createHorseNeighDunhillClauses(5, 4, 4, 2); // Pet: 5, Horses: 4, Cigar: 4, Dunhill: 2
    clauses.insert(clauses.end(), condition11Clause.begin(), condition11Clause.end());
    
    vector<vector<int>> condition12Clause = createImplicationClause(4, 3, 3, 4); // Cigar: 4, Bluemasters: 3, Beverage: 3, Beer: 4
    clauses.insert(clauses.end(), condition12Clause.begin(), condition12Clause.end());

    vector<vector<int>> condition13Clause = createImplicationClause(2, 4, 4, 4); // National: 2, German: 4, Cigar: 4, Prince: 4
    clauses.insert(clauses.end(), condition13Clause.begin(), condition13Clause.end());

    // vector<vector<int>> condition14Clause = createHorseNeighDunhillClauses(2, 5, 1, 3); // National: 2, Norwegian: 5, Color: 1, Blue: 3
    // clauses.insert(clauses.end(), condition14Clause.begin(), condition14Clause.end());

    vector<vector<int>> condition15Clause = createHorseNeighDunhillClauses(4, 2, 3, 1); // Cigar: 4, Blends: 2, Beverage: 3, Water: 1
    clauses.insert(clauses.end(), condition15Clause.begin(), condition15Clause.end());

    /*
        add clause to make dpll faster
    */
    vector<vector<int>> condition16Clause = createSingleTruthClauses(2, 1, 3); // House: 2, Color: 1, Blue: 3
    clauses.insert(clauses.end(), condition16Clause.begin(), condition16Clause.end());

    return clauses;
}

void printCNF(const vector<vector<int>>& clauses) {
    cout << "p cnf " << 125 << " " << clauses.size() << endl;

    // for (const auto& clause : clauses) {
    //     for (int literal : clause) {
    //         cout << literal << ' ';
    //     }
    //     cout << "0" << endl;
    // }
}

vector<vector<int>> simplifyCNF(const vector<vector<int>>& clauses) {
    vector<vector<int>> reducedClauses;

    // Using unordered_set to remove duplicate clauses
    unordered_set<string> uniqueClauses;

    for (const auto& clause : clauses) {
        // Remove duplicate literals in a clause
        unordered_set<int> uniqueLiterals(clause.begin(), clause.end());
        
        // Check if the clause is tautological
        bool isTautological = false;
        for (int literal : uniqueLiterals) {
            if (uniqueLiterals.count(-literal)) {
                isTautological = true;
                break;
            }
        }

        if (isTautological) continue;

        // Create a sorted vector from the unique literals
        vector<int> reducedClause(uniqueLiterals.begin(), uniqueLiterals.end());
        sort(reducedClause.begin(), reducedClause.end());

        // Convert the sorted vector to a string to help identify duplicate clauses
        string clauseStr;
        for (int literal : reducedClause) {
            clauseStr += to_string(literal) + ",";
        }

        // If the clause is unique, add it to the result
        if (uniqueClauses.find(clauseStr) == uniqueClauses.end()) {
            uniqueClauses.insert(clauseStr);
            reducedClauses.push_back(reducedClause);
        }
    }

    return reducedClauses;
}

vector<vector<int>> furtherReduceCNF(const vector<vector<int>>& clauses) {
    vector<vector<int>> furtherReducedClauses;
    unordered_set<int> singleVariableLiterals;

    // First pass: Identify single-variable clauses and their literals
    for (const auto& clause : clauses) {
        if (clause.size() == 1) {
            singleVariableLiterals.insert(clause[0]);
        }
    }

    // Second pass: Remove clauses made redundant by single-variable literals and simplify other clauses
    for (const auto& clause : clauses) {
        vector<int> newClause;
        bool shouldAddClause = true;

        for (const auto& literal : clause) {
            // if literal is a single-variable literal, then the whole clause can be removed.
            if (singleVariableLiterals.find(literal) != singleVariableLiterals.end()) {
                shouldAddClause = false;
                break;
            }
            // if the negation of literal is a single-variable literal, then just skip this literal.
            if (singleVariableLiterals.find(-literal) == singleVariableLiterals.end()) {
                newClause.push_back(literal);
            }
        }

        if (shouldAddClause && !newClause.empty()) {
            furtherReducedClauses.push_back(newClause);
        }
    }

    return furtherReducedClauses;
}

bool DPLL(vector<vector<int>> clauses, unordered_map<int, bool> model);

bool allClausesSatisfied(const vector<vector<int>> &clauses, const unordered_map<int, bool> &model) {
    for (const auto &clause : clauses) {
        bool clauseSatisfied = false;
        for (int literal : clause) {
            int var = abs(literal);
            if (model.find(var) != model.end()) {
                bool value = model.at(var);
                if (literal > 0 && value || literal < 0 && !value) {
                    clauseSatisfied = true;
                    break;
                }
            }
        }
        if (!clauseSatisfied) return false;
    }
    return true;
}

bool DPLL(vector<vector<int>> clauses, unordered_map<int, bool> model) {
    // Check if all clauses are satisfied
    if (allClausesSatisfied(clauses, model)) {
        return true;
    }

    // Select an unassigned variable
    int unassignedVar = 0;
    for (const auto &clause : clauses) {
        for (int literal : clause) {
            int var = abs(literal);
            if (model.find(var) == model.end()) {
                unassignedVar = var;
                break;
            }
        }
        if (unassignedVar) break;
    }
    if (!unassignedVar) return false;

    // Try both true and false for the unassigned variable
    unordered_map<int, bool> modelWithTrue = model;
    modelWithTrue[unassignedVar] = true;
    if (DPLL(clauses, modelWithTrue)) {
        return true;
    }
    
    unordered_map<int, bool> modelWithFalse = model;
    modelWithFalse[unassignedVar] = false;
    if (DPLL(clauses, modelWithFalse)) {
        return true;
    }

    return false;
}


bool graphDPLL(std::vector<std::vector<int>> &clauses, std::unordered_map<int, bool> &assignment) {
    if (clauses.empty()) return true;
    if (std::any_of(clauses.begin(), clauses.end(), [](const std::vector<int>& clause) { return clause.empty(); })) return false;

    // Unit propagation
    std::unordered_set<int> unitClauses;
    for (const auto& clause : clauses) {
        if (clause.size() == 1) unitClauses.insert(clause[0]);
    }
    
    if (!unitClauses.empty()) {
        std::vector<std::vector<int>> newClauses;
        for (const auto& clause : clauses) {
            std::vector<int> newClause;
            for (auto literal : clause) {
                if (unitClauses.count(literal)) goto NextClause;
                if (unitClauses.count(-literal)) continue;
                newClause.push_back(literal);
            }
            newClauses.push_back(newClause);
            NextClause: continue;
        }

        for (auto unit : unitClauses) {
            assignment[std::abs(unit)] = unit > 0;
        }

        return graphDPLL(newClauses, assignment);
    }

    // Choose a literal to branch on
    int literal = std::abs(clauses[0][0]);

    // Try assigning the literal to true
    std::unordered_map<int, bool> newAssignment = assignment;
    newAssignment[literal] = true;

    std::vector<std::vector<int>> newClauses;
    for (const auto& clause : clauses) {
        std::vector<int> newClause;
        for (auto l : clause) {
            if (l == literal) goto Next;
            if (l == -literal) continue;
            newClause.push_back(l);
        }
        newClauses.push_back(newClause);
        Next: continue;
    }

    if (graphDPLL(newClauses, newAssignment)) {
        assignment = newAssignment;
        return true;
    }

    // Try assigning the literal to false
    newAssignment = assignment;
    newAssignment[literal] = false;

    newClauses.clear();
    for (const auto& clause : clauses) {
        std::vector<int> newClause;
        for (auto l : clause) {
            if (l == -literal) goto Skip;
            if (l == literal) continue;
            newClause.push_back(l);
        }
        newClauses.push_back(newClause);
        Skip: continue;
    }

    if (graphDPLL(newClauses, newAssignment)) {
        assignment = newAssignment;
        return true;
    }

    return false;
}

int main()
{
    int no_of_houses = 5;
    vector<string> categories = {"Color", "Nationality", "Beverage", "Cigar", "Pet"};
    vector<vector<string>> values = {
        {"Red", "Green", "Blue", "Yellow", "White"},
        {"Brit", "Swede", "Dane", "German", "Norwegian"},
        {"Tea", "Milk", "Coffee", "Water", "Beer"},
        {"Pall Mall", "Dunhill", "Blends", "Bluemasters", "Prince"},
        {"Birds", "Dogs", "Cats", "Horse", "Fish"}
    };

    for (int i = 1; i <= no_of_houses; i++) {
        for (int j = 1; j <= categories.size(); j++) {
            for (int k = 1; k <= values[0].size(); k++) {
                // for cnf form, create id for each literal 
                int id = getID(i, j, k);
                // cout << "Home " << i << ", Category " << categories[j-1] << ", Value " << values[j-1][k-1] << " => ID: " << id << endl;
            }
        }
    }

    auto [i, j, k] = getValues(84);
    cout << "Original values from ID: Home " << i << ", Category " << categories[j-1] << ", Value " << values[j-1][k-1] << endl;

    // 1, 52, 79, 105, 61, 111, 63, 92, 
    // auto [i, j, k] = getValues(10);
    // cout << "Original values from ID: Home " << i << ", Category " << categories[j-1] << ", Value " << values[j-1][k-1] << endl;

    // auto [i, j, k] = getValues(28);
    // cout << "Original values from ID: Home " << i << ", Category " << categories[j-1] << ", Value " << values[j-1][k-1] << endl;

    // auto [i, j, k] = getValues(84);
    // cout << "Original values from ID: Home " << i << ", Category " << categories[j-1] << ", Value " << values[j-1][k-1] << endl;


    cout << "Literal creations ends here" << endl;

    vector <vector<int>> clauses = createClauses();

    vector <vector<int>> reducedClauses = furtherReduceCNF(simplifyCNF(clauses));

    printCNF(reducedClauses);

    unordered_map<int, bool> model;
    bool result = graphDPLL(reducedClauses, model);
    
    if (result) {
        cout << "SAT\n";
        for (const auto &entry : model) {
            cout << "x" << entry.first << " = " << entry.second << "\n";
        }
    } else {
        cout << "UNSAT\n";
    }

    cout << "CNF and DPLL procedure ends" << endl;
}
