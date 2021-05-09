#include <iostream>

using namespace std;

int *readNumbers(int n){
	int * array = new int[n];
	for (int i = 0; i < n; i++){
		cin >> array[i];
	}
	return array;
}


void grades (int *gradepoints, int length) {
	string val;
	for (int i = 0; i< length; i++){
		switch(gradepoints[i]){
			case 1:
			val = "Fail";
			break;
			case 2:
			val = "Pass";
			break;
			case 3:
			val = "Credit";
			break;
			case 4:
			val = "Distinction";
			break;
			case 5:	
			val = "Higher Distinction";
			break;
		}
		cout << i << " " << gradepoints[i] << " " << val << endl;
	}
}