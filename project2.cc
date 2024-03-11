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

using namespace std;
vector<string> LHSVector;
vector<vector<string>> RHSVector;

vector<string> universe;

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
    unordered_set<string> uniqueItems; // Set to store unique items

    // Print the items in the LHS vector while ensuring uniqueness and order preservation
    for (int i = 0; i < LHSVector.size(); i++) {
        if (uniqueItems.insert(LHSVector.at(i)).second) {
                //cout << LHSVector.at(i) << " ";
                universe.push_back(LHSVector.at(i));

        }
        for(string s : RHSVector.at(i)){
            if (uniqueItems.insert(s).second && contains(LHSVector, s)) {
                //cout << s << " ";
                universe.push_back(s);
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
    
    unordered_set<string> uniqueItems; // Set to store unique items

    // Iterate through RHSVector and insert unique items into the set
    for (int i = 0; i < copiedRHS.size(); i++) {
        for (string item : copiedRHS.at(i)) {
            if (uniqueItems.insert(item).second) {
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
void printGrammar() {
    cout << "Grammar Rules:" << endl;
    for (const auto& rule : grammar) {
        cout << "LHS: " << rule.LHS << ", RHS: ";
        for (int rhsItem : rule.RHS) {
            cout << rhsItem << " ";
        }
        cout << std::endl;
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
    //printGrammar();
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
void RemoveUselessSymbols()
{
    cout << "2\n";
}

// Task 3
void CalculateFirstSets()
{
    cout << "3\n";
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

