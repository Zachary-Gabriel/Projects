#include "Rearrange.h"

Individual* Rearrange::mutate(Individual* input, int k){
	// Preallocating memory and making appropriate variables.
	k--; // Fixing index
	std::string temp =input->getString();
	int length = temp.length();
	int i = k%(length);
	// Moving the back chunk of the front and then adding the other chunk.
	std::string output = temp.substr(i)+temp.substr(0,i);
	Individual* temp1 = new Individual(output);
	return temp1;
}