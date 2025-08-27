#include <iostream>
#include <string>
using namespace std;

string unionArr(int arr1[], int size1, int arr2[], int size2) {
string result = "";
for (int i = 0; i < size1; i++) {
string numStr = to_string(arr1[i]);
if (result.find(numStr) == string::npos) {
if (!result.empty()) result += ",";
result += numStr;
       }
  }
for (int j = 0; j < size2; j++) {
 string numStr = to_string(arr2[j]);
 if (result.find(numStr) == string::npos) {
 if (!result.empty()) result += ",";
 result += numStr;
        }
    }

return result;
 }

string intersectArr(int arr1[], int size1, int arr2[], int size2) {
  string result = "";

for (int i = 0; i < size1; i++) {
for (int j = 0; j < size2; j++) {
if (arr1[i] == arr2[j]) {
string numStr = to_string(arr1[i]);
if (result.find(numStr) == string::npos) {
if (!result.empty()) result += ",";
result += numStr;
           }
       }
    }
}

return result;
 }

int main() {
int arr1[] = {1, 2, 3, 4, 5};
int arr2[] = {2, 4, 6, 8};
cout << "Union: " << unionArr(arr1, 5, arr2, 4) << endl;
cout << "Intersection: " << intersectArr(arr1, 5, arr2, 4) << endl;

return 0;
 }
