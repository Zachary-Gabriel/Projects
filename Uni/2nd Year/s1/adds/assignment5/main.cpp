#include "MapAbsoluteValue.h"
#include "MapSquare.h"
#include "MapTriple.h"
#include "FilterOdd.h"
#include "FilterNonPositive.h" 
#include "FilterForTwoDigitPositive.h"
#include "ReduceMinimum.h"
#include "ReduceGCD.h"

#include <iostream>
#include <vector>

int main(void){
	// Preallocating space in memory
	int i = 0;

	std::string inputText = "";
	std::getline (std::cin,inputText);
	std::vector<int> values;
	
	int temp = 0;
	bool isNegative = false;

	MapTriple triple = MapTriple();
	MapAbsoluteValue abs = MapAbsoluteValue();

	FilterOdd oddFilter = FilterOdd();
	FilterForTwoDigitPositive pos2filter = FilterForTwoDigitPositive();

	ReduceMinimum min = ReduceMinimum();
	ReduceGCD gcd = ReduceGCD();

	// Converting string input into int arrays
	while(inputText[i] != '\0'){

		// Checking if the next number is negative
		if(int(inputText[i]) == 45){
			isNegative = true;
		}

		// For a positive number
		if(isNegative == false && int(inputText[i])>47 && int(inputText[i])<58){
			// Multiplying previous number by ten then adding the new number
			temp = 10*temp;
			temp += int(inputText[i]) - 48;
			if(int(inputText[i+1]) < 48 || int(inputText[i+1]) > 57){
				// Adding new int to the values vector
				values.push_back (temp);
				temp = 0;
			}
		}

		// For a negative number
		if(isNegative == true && int(inputText[i])>47 && int(inputText[i])<58){
			// Multiplying previous number by ten then adding the new number
			temp = 10*temp;
			temp -= int(inputText[i]) - 48;
			if(int(inputText[i+1]) < 48 || int(inputText[i+1]) > 57){
				// Adding new int to the values vector
				values.push_back (temp);
				temp = 0;
				isNegative = false;
			}
		}

		i++;
	}

	if(values.size() != 20){
		std::cout << "ERROR: invalid input size \n";
		return 0;
	}
	
	// Applying the two mapping functions to the values
	values = triple.map(values);
	values = abs.map(values);

	// Applying the two filter functions to the values
	values = oddFilter.filter(values);
	values = pos2filter.filter(values);

	// Applying the two reduction functions to the values
	int output1 = min.reduce(values);
	int output2 = gcd.reduce(values);

	// printing to console
	std::cout<< output1 << " "<< output2 << std::endl;

	return 0;
}