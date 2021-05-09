#include "FilterGeneric.h"

std::vector<int> FilterGeneric::filter(std::vector<int> input){
	// Base case.
	if(input.size() == 0){
		return output;
	}

	// adds to output vecctor if passes true
	// Adds to output vector if g is true
	int temp = g(input.front());
	if(temp == true){
		output.push_back(input.front());
	}
	
	// Iterates
	input.erase(input.begin());
	return filter(input);
}