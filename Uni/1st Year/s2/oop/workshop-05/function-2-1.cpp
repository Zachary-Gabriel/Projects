#include <iostream>

using namespace std;

int * readNumbers() {
	int *array = new int[10];
	for (int i = 0; i < 10; i++) {
		cin >> array[i];
	}
	return array;
}

void hexDigits(int *numbers,int length) {
	char Character;
	for (int i = 0; i < length; i++){
		if(numbers[i] < 10){
			cout << i << " " << numbers[i] << " " << numbers[i] << endl;
		} else {
			switch (numbers[i]) {
				case 10:
				Character = 'A';
				break;
				case 11:
				Character = 'B';
				break;
				case 12:
				Character = 'C';
				break;
				case 13:
				Character = 'D';
				break;
				case 14:
				Character = 'E';
				break;
				case 15:
				Character = 'F';
				break;
			}
			cout << i << " " << numbers[i] << " " << Character << endl; 
		}
	}
}