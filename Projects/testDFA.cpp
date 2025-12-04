#include "DFA.h"
#include <iostream>
#include <cassert>
using namespace std;

void testLoadValidDFA() {
    DFA dfa;
    bool loaded = dfa.loadFromFile("test_dfa_valid.txt");
    assert(loaded && "Valid DFA failed to load");
    cout << "[PASS] testLoadValidDFA\n";
}

void testLoadInvalidDFA() {
    DFA dfa;
    bool loaded = dfa.loadFromFile("test_dfa_invalid.txt");
    assert(!loaded && "Invalid DFA incorrectly loaded");
    cout << "[PASS] testLoadInvalidDFA\n";
}

void testAcceptString() {
    DFA dfa;
    dfa.loadFromFile("test_dfa_valid.txt");

    string accepted = "abba";
    string rejected = "a";

    assert(dfa.runString(accepted) && "String should be accepted");
    assert(!dfa.runString(rejected) && "String should be rejected");

    cout << "[PASS] testAcceptString\n";
}

void testDisplayComputation() {
    DFA dfa;
    dfa.loadFromFile("test_dfa_valid.txt");

    cout << "Display computation for 'abba':\n";
    dfa.displayComputation("abba");
    cout << "[PASS] testDisplayComputation\n";
}

int main() {
    cout << "=== Running DFA Tests ===\n";

    testLoadValidDFA();
    testLoadInvalidDFA();
    testAcceptString();
    testDisplayComputation();

    cout << "All tests passed!\n";
    return 0;
}