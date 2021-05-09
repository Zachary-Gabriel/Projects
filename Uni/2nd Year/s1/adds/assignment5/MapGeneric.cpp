#include "MapGeneric.h"

std::vector<int> MapGeneric::map(std::vector<int> input){
	// Base case.
	if(input.size() == 0){
		return output;
	}
	// Takes front value and applies "f" and adds it to a new vector {output}
	int temp = f(input.front());
	output.push_back(temp);

	// Removes front value then loops again.
	input.erase(input.begin());
	return map(input);
}