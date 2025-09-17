#include <iostream>
using namespace std;

int degree(bool adjMatrix[][100], int size, int vertex) {
  int outDegree = 0;
  int inDegree = 0;
  for (int col = 0; col < size; col++) {
   if (adjMatrix[vertex][col])
   {outDegree++;
        }
    }
    for (int row = 0; row < size; row++) {
        if (adjMatrix[row][vertex]) {
          inDegree++;
        }
    }

  return outDegree + inDegree;
}
