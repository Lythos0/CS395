#ifndef DFA_H
#define DFA_H
#include "Object.h"
#include "Set.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;

class DFA : public tcp::Object
{
public:
    tcp::Set<string> Q; // set of states
    tcp::Set<char> Sigma; // alphabet
    string q0; // initial state
    tcp::Set<string> F; // set of accepting states
    tcp::Set<tcp::Triple> delta; // transition function

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