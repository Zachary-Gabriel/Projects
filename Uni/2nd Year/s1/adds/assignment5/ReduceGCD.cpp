#include "ReduceGCD.h"

int ReduceGCD::binaryOperator(int input1, int input2){
	int temp = input1%input2;
	// Base case
	if(temp==0){
		return input2;
	}
	// Iteration
	return binaryOperator(input2,temp);
}