#include <iostream>

using namespace std;

int *readNumbers(int n){
	int * array = new int[n];
	for (int i = 0; i < n; i++){
		cin >> array[i];
	}
	return array;
}


void printNumbers(int * numbers, int length){
	cout << "{";
	for (int i = 0; i< length; i++){
		cout << numbers[i];
		if (i != length-1){
			cout << ",";
		}
	}
	cout << "}" << endl;
}

void subArrays(int * array, int length) {

	for (int i = 0; i < length; i++){
		for (int j = i+1; j < length; j++){
			int* array1 = new int[j-i];
			for (int k = i; k < j; k++){
				array1[k-i] = array[k]; 
			}
			printNumbers(array1,j-i);
			delete[] array1;
		}
	}
}