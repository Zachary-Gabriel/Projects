#include "BitFlipProb.h"
#include <cmath>

Individual* BitFlipProb::mutate(Individual* input, int k){
	// Storing variables in memory
	std::string tempString = input->getString();
	int length = tempString.length();
	std::string output = "";

	// Looping over the string and swapping randomly
	for(int i = 0; i < length; i++){
		// Making another string with the same elements as the input string 
		// but changing the element of the input index indexOfChange.
		if(rand() % k == 1){
			if (tempString[i] == '0'){
				output += '1';
			} else if (tempString[i] == '1'){
				output += '0';
			} 
		} else {
			output += tempString[i];
		}
	}
	Individual* temp1 = new Individual(output);
	return temp1;
}