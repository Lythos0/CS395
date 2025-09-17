#include <iostream>
using namespace std;

bool isPermutation(int arr1[], int arr2[], int size){
bool used[100];
for(int i = 0; i < size; i++){
  for int j = 0; j < size; j++){
 if (!used[j] && arr1[i] == arr2[j]) {
               matched[j] = true;  
              found = true;
              break;
            }
        }
        if (!found) return false; 
    }
    return true;
}

