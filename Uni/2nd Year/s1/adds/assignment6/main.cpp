#include "Individual.h"
#include "Mutator.h"
#include "Rearrange.h"
#include "BitFlipProb.h"
#include "BitFlip.h"

#include <sstream>
#include <iostream>

// Applies mutate function
Individual* execute(Individual* indPtr, Mutator* mPtr, int k){
	Individual* output = mPtr->mutate(indPtr,k);
	return output;
}

int main(void){
	// Initialising variables in memory
	std::string input, binarystr1,binarystr2; 
	int k1, k2;
	bool isInt;
	BitFlip *flipObject = new BitFlip();
	Rearrange *rearrangeObject = new Rearrange();

	// Converting input to variables using streamline
	std::getline(std::cin, input);
	std::stringstream ss;
	ss << input;
	ss >> binarystr1 >> k1 >> binarystr2 >> k2;

	// Creating objects out of input
	Individual* indivOne;
	Individual* indivTwo;

	// Making first object
	// Checking to see if int or string
	isInt = false;
	for(int i = 0; i < binarystr1.length(); i++){
		if (binarystr1[i] == '0' || binarystr1[i] == '1'){
		} else {
			isInt = true;

		}
	}
	// Creating Individual object
	if(isInt == false){
		indivOne = new Individual(binarystr1);
	} else {
		int length = std::stoi(binarystr1);
		indivOne = new Individual(length);
	}

	// Making second object
	// Checking to see if int or string
	isInt = false;
	for(int i = 0; i < binarystr2.length(); i++){
		if (binarystr2[i] == '0' || binarystr2[i] == '1'){
		} else {
			isInt = true;
		}
	}
	// Creating Individual object
	if(isInt == false){
		indivTwo = new Individual(binarystr2);
	} else {
		int length = std::stoi(binarystr2);
		indivTwo = new Individual(length);
	}

	// Mutating Individuals
	Individual* mutateOne = execute(indivOne, flipObject, k1);
	Individual* mutateTwo = execute(indivTwo, rearrangeObject, k2);

	//Printing to console
	std::cout << mutateOne->getString()  << " " << mutateTwo->getString() <<  " " << mutateTwo->getMaxOnes() <<std::endl;
	return 0;
}