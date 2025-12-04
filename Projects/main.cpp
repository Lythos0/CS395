#include <iostream>
#include <string>
#include <sstream>
#include "DFA.h"

using namespace std;

int main() {
    DFA dfa;
    string command;

    cout << "DFA Simulator" << endl;
    cout << "Commands:" << endl;
    cout << "  load <file>     - Load DFA from file" << endl;
    cout << "  readstring <str> - Test string acceptance" << endl;
    cout << "  visualize <str>  - Display computation for string" << endl;
    cout << "  display         - Show DFA formal representation" << endl;
    cout << "  exit            - Exit the simulator" << endl;
    cout << endl;

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command.empty()) continue;

        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "load") {
            string filename;
            ss >> filename;
            if (filename.empty()) {
                cout << "Error: Please provide a filename" << endl;
                continue;
            }
            if (dfa.loadFromFile(filename)) {
                cout << "DFA loaded successfully from " << filename << endl;
            } else {
                cout << "Failed to load DFA from " << filename << endl;
            }
        } else if (cmd == "readstring") {
            string input;
            getline(ss, input);
            // Remove leading space
            if (!input.empty() && input[0] == ' ') input = input.substr(1);
            if (input.empty()) {
                cout << "Error: Please provide a string to test" << endl;
                continue;
            }
            if (dfa.runString(input)) {
                cout << "String accepted!" << endl;
            } else {
                cout << "String rejected." << endl;
            }
        } else if (cmd == "visualize") {
            string input;
            getline(ss, input);
            // Remove leading space
            if (!input.empty() && input[0] == ' ') input = input.substr(1);
            if (input.empty()) {
                cout << "Error: Please provide a string to visualize" << endl;
                continue;
            }
            dfa.displayComputation(input);
        } else if (cmd == "display") {
            dfa.displayFormal();
        } else if (cmd == "exit") {
            break;
        } else {
            cout << "Unknown command: " << cmd << endl;
            cout << "Available commands: load, readstring, visualize, display, exit" << endl;
        }
    }

    return 0;
}
