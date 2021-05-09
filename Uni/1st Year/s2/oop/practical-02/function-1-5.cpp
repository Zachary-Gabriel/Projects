#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;


void print_summed_matrices(int array1[3][3],int array2[3][3]){
	int newArray [3][3];
	int space = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			newArray[i][j] = array1[i][j]+array2[i][j];
			cout << newArray[i][j];
			if(j== 2 && i !=2){
				cout << endl;
			} else {
				cout << " ";
			}
		}
	}
	cout << endl;
}