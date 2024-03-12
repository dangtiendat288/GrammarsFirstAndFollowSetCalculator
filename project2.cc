/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;
vector<string> LHSVector;
vector<vector<string>> RHSVector;

unordered_set<string> uniqueRHSItems; // Set to store unique RHS items
unordered_set<string> uniqueLHSItems; // Set to store unique items

vector<string> universe;

unordered_map<int, unordered_set<int>> first;

struct Rule {
    int LHS;
    vector<int> RHS;
};

vector<Rule> grammar;

bool contains(vector<string> vec, string str) {
    for (string element : vec) {
        if (element == str) {
            return true;
        }
    }
    return false;
}

void addLHS() {

    // Print the items in the LHS vector while ensuring uniqueness and order preservation
    for (int i = 0; i < LHSVector.size(); i++) {
        if (uniqueLHSItems.insert(LHSVector.at(i)).second) {
                //cout << LHSVector.at(i) << " ";
                universe.push_back(LHSVector.at(i));

        }
        for(string s : RHSVector.at(i)){
            if(contains(LHSVector, s)){
                if (uniqueLHSItems.insert(s).second) {
                    //cout << s << " ";
                    universe.push_back(s);
                }
            }
        }
    }
}

void addRHS() {

    // Declare a copy of RHSVector
    vector<vector<string>> copiedRHS = RHSVector;

    // Remove common elements from RHSVector
    for (auto iter = copiedRHS.begin(); iter != copiedRHS.end(); ++iter) {
        for (auto sIter = iter->begin(); sIter != iter->end();) {
            if (contains(LHSVector, *sIter) || *sIter == "#") {
                sIter = iter->erase(sIter);
            } else {
                ++sIter;
            }
        }
    }
    
    // Iterate through RHSVector and insert unique items into the set
    for (int i = 0; i < copiedRHS.size(); i++) {
        for (string item : copiedRHS.at(i)) {
            if (uniqueRHSItems.insert(item).second) {
                //cout << item << " ";
                universe.push_back(item);
            }
        }
    }
}

// Function to get the index of a string in a vector of strings
// Returns -1 if the string is not found
int getIndex(const vector<string>& vec, const string& target) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == target) {
            return i; // Return the index if found
        }
    }
    return -1; // Return -1 if not found
}

void createGrammar(){
    for(int i = 0; i < LHSVector.size(); i++){
        Rule r;
        r.LHS = getIndex(universe, LHSVector.at(i));
        vector<int> tempVec;
        for(int j = 0; j < RHSVector.at(i).size(); j++){
            tempVec.push_back(getIndex(universe, RHSVector.at(i).at(j)));
        }
        r.RHS = tempVec;
        grammar.push_back(r);
    }
}

// Function to print the grammar rule by rule
void printGrammar(vector<Rule> mGrammar) {
    cout << "Grammar Rules:" << endl;
    for (const auto& rule : mGrammar) {
        cout << "LHS: " << rule.LHS << ", RHS: ";
        for (int rhsItem : rule.RHS) {
            cout << rhsItem << " ";
        }
        cout << endl;
    }
}

void printStringGrammar(vector<Rule> grammar) {
    for (const auto& rule : grammar) {
        cout << universe[rule.LHS] << " -> ";
        for (size_t i = 0; i < rule.RHS.size(); ++i) {
            cout << universe[rule.RHS[i]];
            if (i < rule.RHS.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

// read grammar
void ReadGrammar()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();

    LHSVector.push_back(token.lexeme);

    while (token.token_type != END_OF_FILE) {
        token = lexer.GetToken();
        
        if (lexer.peek(1).token_type == ARROW) {
            LHSVector.push_back(token.lexeme);
            continue;
        }

        if (token.token_type == ARROW) {
            if(lexer.peek(1).token_type == STAR){
                vector<string> tempRHSVector; // Temporary vector to hold RHS tokens
                tempRHSVector.push_back("#");

                // Push the temporary RHS vector into the 2D RHS vector
                RHSVector.push_back(tempRHSVector);
            }
            continue;
        }

        if (token.token_type != STAR && token.token_type != END_OF_FILE && token.token_type != HASH) {
            vector<string> tempRHSVector; // Temporary vector to hold RHS tokens
            tempRHSVector.push_back(token.lexeme); // Push the current token into temporary RHS vector
            
            // Loop to populate temporary RHS vector until encountering STAR or END_OF_FILE
            while (lexer.peek(1).token_type != STAR && lexer.peek(1).token_type != END_OF_FILE) {
                token = lexer.GetToken();
                tempRHSVector.push_back(token.lexeme);
            }
            
            // Push the temporary RHS vector into the 2D RHS vector
            RHSVector.push_back(tempRHSVector);
        }

    }

    addRHS();
    addLHS();
    createGrammar();
    //printGrammar(grammar);
}

void printUniverse(){
    if(universe.size() >= 2){
        for(int i = 2; i < universe.size(); i++){
            cout << universe.at(i);
            if(i != universe.size() - 1){
                cout << " ";
            }
        }
    }
}
// Task 1
void printTerminalsAndNoneTerminals()
{
    printUniverse();
}

// Task 2
void RemoveUselessSymbols() {
    // Use this vector to determine whether a symbol is generating or not
    vector<bool> generatingVec(universe.size(), false);

    // epsilon is generating
    generatingVec.at(0) = true;

    // Set all terminals to be generating
    for (int i = 2; i < uniqueRHSItems.size() + 2; i++) {
        if (universe.size() > i) {
            generatingVec.at(i) = true;
        }
    }    

    // for(bool a : generatingVec){
    //     cout << a << " ";
    // }
    // cout << endl;

    bool changed = true; // Initialize changed to true to enter the loop

    while (changed) {
        changed = false; // Reset changed flag at the beginning of each iteration
        for (const auto& rule : grammar) {
            if (generatingVec.at(rule.LHS)) {
                continue; // Skip rules where LHS is already generating
            }
            bool allGenerating = true; // Assume all RHS items are generating initially
            for (int rhsItem : rule.RHS) {
                // Check if any of the RHS items is non-generating
                if (!generatingVec.at(rhsItem)) {
                    allGenerating = false;
                    break; // No need to continue checking the rest of RHS items
                }
            }
            if (allGenerating) {
                // If all RHS items are generating, mark LHS as generating
                generatingVec.at(rule.LHS) = true;
                changed = true; // Set changed to true if any change is made
            }
        }
    }

    // Remove non-generating rules
    vector<Rule> generatingGrammar;

    for (const auto& rule : grammar) {
        bool allGeneratingRHS = true;
        for(int item : rule.RHS){
            if(!generatingVec.at(item)){
                allGeneratingRHS = false;
                break;
            }
        }

        if (generatingVec.at(rule.LHS) && allGeneratingRHS) {
            generatingGrammar.push_back(rule);
        }
    }

    // Print generating grammar
    // cout << "Generating Grammar: \n";
    // printGrammar(generatingGrammar);

    // Use this vector to determine whether a symbol is reachable or not
    vector<bool> reachableVec(universe.size(), false);

    // epsilon is reachable
    reachableVec.at(0) = true;

    // Start non-terminal is reachable
    if (!uniqueLHSItems.empty()) {
        reachableVec.at(2 + uniqueRHSItems.size()) = true;
    }

    bool c = true; // Initialize c to true to enter the loop

    while (c) {
        c = false; // Reset c flag at the beginning of each iteration
        for (const auto& rule : generatingGrammar) {
            if (!reachableVec.at(rule.LHS)) {
                continue; // Skip rules where LHS is not reachable
            }
            bool changeMade = false; // Flag to track if any changes are made for the current rule
            for (int rhsItem : rule.RHS) {
                if (!reachableVec.at(rhsItem)) {
                    reachableVec.at(rhsItem) = true; // Update reachableVec if RHS item is not yet reachable
                    changeMade = true; // Set changeMade to true if any change is made
                }
            }
            if (changeMade) {
                c = true; // Set c to true if any change is made for the current rule
            }
        }
    }

    // Remove all non-reachable rules
    vector<Rule> usefulGrammar;

    for (const auto& rule : generatingGrammar) {
        bool allReachableRHS = true;
        for (int rhsItem : rule.RHS) {
            if (!reachableVec.at(rhsItem)) {
                allReachableRHS = false;
                break;
            }
        }
        if (reachableVec.at(rule.LHS) && allReachableRHS) {
            usefulGrammar.push_back(rule);
        }
    }

    // Print useful grammar
    // cout << "Useful Grammar: \n";
    printStringGrammar(usefulGrammar);
}

//printMap
void printMap(unordered_map<int, unordered_set<int>>& map){
    set<int> keys;

    // Copy keys from the map to the set
    for (const auto& pair : map) {
        keys.insert(pair.first);
    }

    // Iterate over sorted keys and print corresponding key-value pairs
    for (int key : keys) {
        // Check if the key meets the condition
        if (key >= 2 + uniqueRHSItems.size()) {
            cout << "FIRST(" << universe[key] << ") = { ";
            
            //sort values
            set<int> values;
            for (int value : map.at(key)) {
                values.insert(value);
            }

            bool firstValue = true;
            for (int value : values) {
                if (!firstValue) {
                    cout << ", "; // Print separator if not the first value
                } else {
                    firstValue = false; // Update flag after printing the first value
                }
                cout << universe[value];
            }
            cout << " }" << endl;
        }
    }
}

bool addItemsWithoutEps(unordered_set<int>& destSet, const unordered_set<int>& sourceSet) {
    bool changed = false;
    for (const auto& item : sourceSet) {
        if (item != 0 && destSet.insert(item).second) {
            changed = true;
        }
    }
    return changed;
}

bool containsValue(const unordered_set<int>& mySet, int value) {
    return mySet.find(value) != mySet.end();
}

// Task 3
void CalculateFirstSets()
{
    //using map for first
    //key = int of non-terminal on the universe
    //value = first set of that key

    //create empty first set for every LHS of the grammar
    for(Rule r : grammar){
        first[r.LHS] = unordered_set<int>();
    }

    //first of epsilon is epsilon
    first[0].insert(0);

    //first of terminals
    for (int i = 2; i < uniqueRHSItems.size() + 2; i++) {
        first[i].insert(i);
    }
    
    //first set calculation
    bool changed = 1;
    while(changed){
        //set changed to false;
        changed = 0;

        //iterate through every rule
        for(Rule r : grammar){
            //iterate through RHS
            for (int i = 0; i < r.RHS.size();) {
                // Check if all previous first[item] contain epsilon
                bool allPreviousContainEpsilon = true;
                for (int j = 0; j < i; ++j) {
                    if (!containsValue(first[r.RHS.at(j)], 0)) {
                        allPreviousContainEpsilon = false;
                        break;
                    }
                }
                
                // Add first(first_item_in_RHS)\{epsilon} to first(LHS) if all previous contain 0
                if (i == 0 || allPreviousContainEpsilon) {
                    //if any item added to LHS, there is a change
                    if (addItemsWithoutEps(first[r.LHS], first[r.RHS.at(i)])) {
                        changed = true;
                    }
                }

                //Check if epsilon appears at the first[last RHS item] and all previous first also contain epsilon
                if(allPreviousContainEpsilon && (i == r.RHS.size() - 1) && (containsValue(first[r.RHS.at(i)], 0))){
                    if (first[r.LHS].insert(0).second) {
                    changed = true;
                    }
                }
                // Increment i
                ++i;
            }            
        }
    }

    printMap(first);
}

// Task 4
void CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main (int argc, char* argv[])
{
    universe.push_back("#");
    universe.push_back("$");

    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

