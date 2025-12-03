#ifndef DFA_H
#define DFA_H
#include "Object.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
using namespace std;

class DFA : public tcp::Object
{
private:
    vector<string> Q; // set of states
    vector<char> Sigma; // alphabet
    string q0; // initial state
    vector<string> F; // set of accepting states
    map<pair<string,char>, string> delta; // transition function

public:
    DFA(); 

    // Loads DFA from file
    bool loadFromFile(const string& filename);

    // Validate the DFA structure
    bool isValid() const;

    // Display the formal definition of the DFA
    void displayFormal() const;

    // Run a string through the DFA and return if accepted
    bool runString(const string& input) const;

    // Display the computation (state transitions) for a string
    void displayComputation(const string& input) const;

    // Override Object's toString method for display purposes
    string toString() const override;
};

#endif
