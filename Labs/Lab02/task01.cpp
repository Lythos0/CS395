#include <iostream>
using namespace std;

bool isSet(int arr[], int size){

for(int i = 0; i < size; i++){
for(int j = i + 1; j < size; j++){
  if(arr[i] == arr[j]){
return false;
  }
}
}
return true;
}
