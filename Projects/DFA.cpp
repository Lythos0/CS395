#include "DFA.h"
#include "Set.h"
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;


DFA::DFA() {
    // Sets are already initialized by their constructors
    q0 = "";
}

// Loads DFA from file 
bool DFA::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }

    Q = tcp::Set<string>(); // Clear sets
    Sigma = tcp::Set<char>();
    F = tcp::Set<string>();
    delta = tcp::Set<tcp::Triple>();
    q0 = "";

    string line;
    int step = 0; // track which section we are in: 0=Q,1=Sigma,2=q0,3=F,4=delta
    while (getline(file, line)) {
        // Removes whitespace at start/end
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty()) continue; // skip empty lines

        if (step == 0) {
            // Q line: space-delimited states
            istringstream iss(line);
            string state;
            while (iss >> state) {
                // validate state: letters, digits, underscores
                bool valid = true;
                for (char c : state) {
                    if (!isalnum(c) && c != '_') {
                        valid = false;
                        break;
                    }
                }
                if (!valid) {
                    cerr << "Invalid state name: " << state << endl;
                    return false;
                }
                Q.insert(state);
            }
            if (Q.size() == 0) {
                cerr << "Error: No states defined in Q" << endl;
                return false;
            }
            step++;
        } else if (step == 1) {
            // Sigma line: symbols, each character is a symbol
            for (char c : line) {
                if (isalnum(c)) {
                    Sigma.insert(c);
                }
            }
            if (Sigma.size() == 0) {
                cerr << "Error: No symbols in Sigma" << endl;
                return false;
            }
            step++;
        } else if (step == 2) {
            // q0 line: initial state
            q0 = line;
            step++;
        } else if (step == 3) {
            // F line: final states
            istringstream iss(line);
            string state;
            while (iss >> state) {
                F.insert(state);
            }
            step++;
        } else if (step == 4) {
            // δ lines: transitions, format: start symbol end
            istringstream iss(line);
            string start, end;
            char symbol;
            if (!(iss >> start >> symbol >> end)) {
                cerr << "Invalid transition line: " << line << endl;
                return false;
            }
            delta.insert(tcp::Triple(start, symbol, end));
        }
    }

    file.close();
    return isValid();
}

//Validation - UPDATED WITH DETERMINISM CHECK
bool DFA::isValid() const {
    // Check q0 in Q
    if (!Q.contains(q0)) {
        cerr << "Invalid DFA: q0 not in Q" << endl;
        return false;
    }

    // Check F ⊆ Q
    for (size_t i = 0; i < F.size(); i++) {
        if (!Q.contains(F[i])) {
            cerr << "Invalid DFA: F contains state not in Q: " << F[i] << endl;
            return false;
        }
    }

    // Check all delta keys valid
    for (size_t i = 0; i < delta.size(); i++) {
        tcp::Triple t = delta[i];
        string start = t.first();
        char symbol = t.second();
        string end = t.third();

        if (!Q.contains(start)) {
            cerr << "Invalid DFA: transition from state not in Q: " << start << endl;
            return false;
        }
        if (!Q.contains(end)) {
            cerr << "Invalid DFA: transition to state not in Q: " << end << endl;
            return false;
        }
        if (!Sigma.contains(symbol)) {
            cerr << "Invalid DFA: transition uses symbol not in Sigma: " << symbol << endl;
            return false;
        }
    }

    // CRITICAL FIX: Check that δ is deterministic (no duplicate (state, symbol) pairs)
    for (size_t i = 0; i < delta.size(); i++) {
        for (size_t j = i + 1; j < delta.size(); j++) {
            tcp::Triple t1 = delta[i];
            tcp::Triple t2 = delta[j];
            if (t1.first() == t2.first() && t1.second() == t2.second()) {
                cerr << "Invalid DFA: Non-deterministic transition from state " 
                     << t1.first() << " on symbol " << t1.second() << endl;
                cerr << "  Transition 1: δ(" << t1.first() << ", " << t1.second() 
                     << ") = " << t1.third() << endl;
                cerr << "  Transition 2: δ(" << t2.first() << ", " << t2.second() 
                     << ") = " << t2.third() << endl;
                return false;
            }
        }
    }

    return true;
}

//Display Formal DFA
void DFA::displayFormal() const {
    cout << "Q: " << Q << endl;
    cout << "Σ: " << Sigma << endl;
    cout << "q0: " << q0 << endl;
    cout << "F: " << F << endl;
    
    cout << "δ: " << endl;
    for (size_t i = 0; i < delta.size(); i++) {
        tcp::Triple t = delta[i];
        cout << t.first() << " " << t.second() << " " << t.third() << endl;
    }
}

//Run String
bool DFA::runString(const string& input) const {
    string current = q0;
    for (char c : input) {
        // Find transition for current state and symbol
        bool found = false;
        for (size_t i = 0; i < delta.size(); i++) {
            tcp::Triple t = delta[i];
            if (t.first() == current && t.second() == c) {
                current = t.third();
                found = true;
                break;
            }
        }
        if (!found) return false; // no valid transition
    }
    return F.contains(current);
}

// Display Computation
void DFA::displayComputation(const string& input) const {
    string current = q0;
    cout << "Initial state: " << current << endl;
    for (size_t i = 0; i < input.length(); i++) {
        char c = input[i];
        
        // Find transition for current state and symbol
        bool found = false;
        tcp::Triple foundTransition;
        for (size_t j = 0; j < delta.size(); j++) {
            tcp::Triple t = delta[j];
            if (t.first() == current && t.second() == c) {
                foundTransition = t;
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "No transition from state " << current << " on symbol " << c << endl;
            return;
        }
        
        cout << "δ(" << current << ", " << c << ") -> " << foundTransition.third() << endl;
        current = foundTransition.third();
    }
    
    if (F.contains(current))
        cout << "String accepted. Final state: " << current << endl;
    else
        cout << "String rejected. Final state: " << current << endl;
}

//toString 
string DFA::toString() const {
    ostringstream oss;
    oss << "DFA Formal Representation:\n";
    oss << "Q: " << Q << "\n";
    oss << "Σ: " << Sigma << "\n";
    oss << "q0: " << q0 << "\n";
    oss << "F: " << F << "\n";
    oss << "δ:\n";
    for (size_t i = 0; i < delta.size(); i++) {
        tcp::Triple t = delta[i];
        oss << t.first() << " " << t.second() << " " << t.third() << "\n";
    }
    return oss.str();
}