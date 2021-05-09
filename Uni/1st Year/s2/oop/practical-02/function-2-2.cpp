#include <cmath>

int binary_to_number(int binary_digets[], int number_of_digets){
	int dec = 0;

	for (int i = 0; i < number_of_digets; i++) {
		dec += binary_digets[i]*pow(2,number_of_digets-1-i);
	}

	return dec;
}