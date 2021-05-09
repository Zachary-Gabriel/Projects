#include "BitFlip.h"
Individual* BitFlip::mutate(Individual* input, int k){

	// Creating relevant variables and storing them in memory
	std::string inputString = input->getString();
	std::string output = "";
	int length = inputString.length();
	k--; // Because the question was written by a disabled child
	int indexOfChange = k%(length) ;

	// Making another string with the same elements as the input string 
	// but changing the element of the input index indexOfChange.
	for (int i = 0; i < length; i++){
		if (inputString[i] == '0' && indexOfChange==i){
			output += '1';
		} else if (inputString[i] == '1' && indexOfChange==i){
			output += '0';
		} else {
			output+=inputString[i];
		}
	}
	Individual* temp = new Individual(output);
	return temp;
}