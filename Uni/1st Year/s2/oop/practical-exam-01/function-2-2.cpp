#include <cmath>

int octal_to_decimal(int octal_digits[], int number_of_digits){
	// declairing variable
	int decimal = 0;

	for (int i = 0; i < number_of_digits; i++){
		// number multiplied by power relevant power of 8
		decimal+= octal_digits[i] * pow(8,number_of_digits-1-i);
	}
	return decimal;
}