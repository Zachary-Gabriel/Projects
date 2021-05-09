#include "ReduceGeneric.h"

// Reduce 
int ReduceGeneric::reduce(std::vector<int> input){
	
	// Base case.
	if(input.size() < 1){
		return -1;
	}
	if(input.size() == 1){
		return input.back();
	}
	if(input.size() == 2){
		return binaryOperator(input.front(),input.back());
	}

	// Takes two inputs from the vector and applies the binary opperator function on them then adds that back onto the vector
	int temp1 = input.back();
	input.pop_back();
	int temp2 = input.back();
	input.pop_back();
	input.push_back(binaryOperator(temp1,temp2));

	// Iteration
	return reduce(input);
}