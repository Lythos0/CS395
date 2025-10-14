#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


void matrixview(bool graph[][100], unsigned int vertices) {
    cout << "  ";
    for (unsigned int i = 0; i < vertices; i++) {
        cout << i + 1 << " ";
    }
    cout << endl;
    
    for (unsigned int i = 0; i < vertices; i++) {
        cout << i + 1 << " ";
        for (unsigned int j = 0; j < vertices; j++) {
            if (graph[i][j]) {
                cout << "O ";
            } else {
                cout << "X ";
            }
        }
        cout << endl;
    }
}

bool undirectedgraph(bool graph[][100], unsigned int vertices) {
    for (unsigned int i = 0; i < vertices; i++) {
        for (unsigned int j = 0; j < vertices; j++) {
            if (graph[i][j] != graph[j][i]) {
                return false;
            }
        }
    }
    return true;
}

void degrees(bool graph[][100], unsigned int vertices) {
    for (unsigned int i = 0; i < vertices; i++) {
        int degree = 0;
        for (unsigned int j = 0; j < vertices; j++) {
            if (graph[i][j]) {
                degree++;
            }
        }
        cout << "deg(" << i + 1 << ") = " << degree << endl;
    }
}

