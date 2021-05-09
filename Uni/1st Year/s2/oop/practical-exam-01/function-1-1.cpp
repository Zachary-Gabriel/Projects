#include <iostream>

using namespace std;

void printer(int array[10][10]){

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			cout << array[i][j]; // prints array
			if (j%10 == 9){
				cout << endl; // creates new line at end of each row
			} else {
				cout << " "; // creates a space after each character
			}
		}
	}

}