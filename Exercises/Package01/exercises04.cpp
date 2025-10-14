#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// 1. AND gate
bool AND(bool a, bool b) {
    return a && b;
}

// 2. OR gate  
bool OR(bool a, bool b) {
    return a || b;
}

// 3. NOT gate
bool NOT(bool a) {
    return !a;
}

// 4.
bool IMP(bool a, bool b) {
    return !a || b; 
}

// 5.
bool XOR(bool a, bool b) {
    return a != b;
}

// 6.
bool EQU(bool a, bool b) {
    return a == b;
}

// 7.
bool E1(bool A, bool B, bool C) {
    return (A && B) || (!C);
}

// 8.
bool E2(bool A, bool B, bool C) {
    return (A || B) && (!A || C);
}

// 9.
bool E3(bool A, bool B, bool C) {
    return A && IMP(B, C);
}

// 10.
bool E4(bool A, bool B, bool C) {
    return IMP(!A, !B) && B && IMP(A, C);
