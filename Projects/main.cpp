#include "DFA.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    DFA dfa;
    string command;

    cout << "=== DFA Simulator ===\n";
    cout << "Commands: load <file>, readstring <string>, display, visualize, exit\n";

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command.size() >= 4 && command.substr(0, 4) == "load") {
            string filename = command.size() > 5 ? command.substr(5) : "";
            if (!filename.empty() && dfa.loadFromFile(filename)) {
                cout << "DFA loaded successfully from " << filename << endl;
            } else {
                cout << "Failed to load DFA from " << filename << endl;
            }
        } 
        else if (command.size() >= 10 && command.substr(0, 10) == "readstring") {
            string input = command.size() > 11 ? command.substr(11) : "";
            if (!input.empty() && dfa.runString(input)) {
                cout << "String accepted!\n";
            } else {
                cout << "String rejected.\n";
            }
        } 
        else if (command == "display") {
            cout << dfa;
        } 
        else if (command.size() >= 9 && command.substr(0, 9) == "visualize") {
            string input = command.size() > 10 ? command.substr(10) : "";
            if (!input.empty()) {
                dfa.displayComputation(input);
            } else {
                cout << "Usage: visualize <string>\n";
            }
        } 
        else if (command == "exit") {
            break;
        } 
        else {
            cout << "Unknown command.\n";
        }
    }

    return 0;
}
