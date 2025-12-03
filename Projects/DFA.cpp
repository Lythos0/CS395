#include "DFA.h"
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;


DFA::DFA() {
    Q.clear();
    Sigma.clear();
    q0 = "";
    F.clear();
    delta.clear();
}

// Loads DFA from file 
bool DFA::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }

    Q.clear();
    Sigma.clear();
    F.clear();
    delta.clear();
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
            set<string> unique;
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
                unique.insert(state);
            }
            if (unique.empty()) {
                cerr << "Error: No states defined in Q" << endl;
                return false;
            }
            Q.assign(unique.begin(), unique.end());
            step++;
        } else if (step == 1) {
            // Sigma line: symbols, each character is a symbol
            for (char c : line) {
                if (isalnum(c)) {
                    if (find(Sigma.begin(), Sigma.end(), c) == Sigma.end())
                        Sigma.push_back(c);
                }
            }
            if (Sigma.empty()) {
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
            set<string> unique;
            while (iss >> state) {
                unique.insert(state);
            }
            F.assign(unique.begin(), unique.end());
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
            delta[{start, symbol}] = end;
        }
    }

    file.close();
    return isValid();
}

//Validation 
bool DFA::isValid() const {
    // Check q0 in Q
    if (find(Q.begin(), Q.end(), q0) == Q.end()) {
        cerr << "Invalid DFA: q0 not in Q" << endl;
        return false;
    }

    // Check F ⊆ Q
    for (string f : F) {
        if (find(Q.begin(), Q.end(), f) == Q.end()) {
            cerr << "Invalid DFA: F contains state not in Q: " << f << endl;
            return false;
        }
    }

    // Check all delta keys valid
    for (auto &t : delta) {
        string start = t.first.first;
        char symbol = t.first.second;
        string end = t.second;

        if (find(Q.begin(), Q.end(), start) == Q.end()) {
            cerr << "Invalid DFA: transition from state not in Q: " << start << endl;
            return false;
        }
        if (find(Q.begin(), Q.end(), end) == Q.end()) {
            cerr << "Invalid DFA: transition to state not in Q: " << end << endl;
            return false;
        }
        if (find(Sigma.begin(), Sigma.end(), symbol) == Sigma.end()) {
            cerr << "Invalid DFA: transition uses symbol not in Sigma: " << symbol << endl;
            return false;
        }
    }

    return true;
}

//Display Formal DFA
void DFA::displayFormal() const {
    cout << "Q: ";
    for (auto s : Q) cout << s << " ";
    cout << endl;

    cout << "Σ: ";
    for (auto c : Sigma) cout << c << " ";
    cout << endl;

    cout << "q0: " << q0 << endl;

    cout << "F: ";
    for (auto f : F) cout << f << " ";
    cout << endl;

    cout << "δ: " << endl;
    for (auto &t : delta) {
        cout << t.first.first << " " << t.first.second << " " << t.second << endl;
    }
}

//Run String
bool DFA::runString(const string& input) const {
    string current = q0;
    for (char c : input) {
        auto it = delta.find({current, c});
        if (it == delta.end()) return false; // no valid transition
        current = it->second;
    }
    return find(F.begin(), F.end(), current) != F.end();
}

// Display Computation
void DFA::displayComputation(const string& input) const {
    string current = q0;
    cout << "Initial state: " << current << endl;
    for (size_t i = 0; i < input.length(); i++) {
        char c = input[i];
        auto it = delta.find({current, c});
        if (it == delta.end()) {
            cout << "No transition from state " << current << " on symbol " << c << endl;
            return;
        }
        cout << "δ(" << current << ", " << c << ") -> " << it->second << endl;
        current = it->second;
    }
    if (find(F.begin(), F.end(), current) != F.end())
        cout << "String accepted. Final state: " << current << endl;
    else
        cout << "String rejected. Final state: " << current << endl;
}

//toString 
string DFA::toString() const {
    ostringstream oss;
    oss << "DFA Formal Representation:\n";
    oss << "Q: ";
    for (auto s : Q) oss << s << " ";
    oss << "\nΣ: ";
    for (auto c : Sigma) oss << c << " ";
    oss << "\nq0: " << q0;
    oss << "\nF: ";
    for (auto f : F) oss << f << " ";
    oss << "\nδ:\n";
    for (auto &t : delta)
        oss << t.first.first << " " << t.first.second << " " << t.second << "\n";
    return oss.str();
}
