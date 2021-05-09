#include <iostream>

extern int octal_to_decimal(int octal_digits[], int number_of_digits);

using namespace std;

int main(void){
	
	// creating numbers
	int octal[2] = {5,7};
	int numbers = 2;

	// running test
	cout << octal << " octal to decimal: " << octal_to_decimal(octal,numbers) << endl;

	return 0;
}