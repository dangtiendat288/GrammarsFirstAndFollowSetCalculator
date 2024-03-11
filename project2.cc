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

bool contains(vector<string> vec, string str) {
    for (string element : vec) {
        if (element == str) {
            return true;
        }
    }
    return false;
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
}

void printLHS() {
    unordered_set<string> uniqueItems; // Set to store unique items

    // Print the items in the LHS vector while ensuring uniqueness and order preservation
    for (string item : LHSVector) {
        if (uniqueItems.insert(item).second) {
            cout << item << " ";
        }
    }
}

void printRHS() {

    // Declare a copy of RHSVector
    vector<vector<string>> copiedRHS = RHSVector;

    // Remove common elements from RHSVector
    for (int i = 0; i < copiedRHS.size(); i++) {
        for(int j = 0; j < copiedRHS.at(i).size(); j++){
            if (contains(LHSVector, copiedRHS.at(i).at(j)) || copiedRHS.at(i).at(j) == "#") {
                copiedRHS.at(i).erase(copiedRHS.at(i).begin() + j);
            }
        }
    }

    unordered_set<string> uniqueItems; // Set to store unique items

    // Iterate through RHSVector and insert unique items into the set
    for (int i = 0; i < copiedRHS.size(); i++) {
        for (string item : copiedRHS.at(i)) {
            if (uniqueItems.insert(item).second) {
                cout << item << " ";
            }
        }
    }
}

// Task 1
void printTerminalsAndNoneTerminals()
{
    printRHS();
    printLHS();
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

