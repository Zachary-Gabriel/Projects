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
	for (int i = 0; i< length; i++){
		cout << i << " " << numbers[i] << endl;
	}
}

int* mergeArrays(int* A, int* B, int length){
	int * array = new int[2*length];
	for (int i = 0; i < length; i++){
		array[i]=A[i];
	}
	for (int i = 0; i < length; i++){
		array[i+length] = B[i];
	}
	return array;
}