#include "ReduceMinimum.h"

int ReduceMinimum::binaryOperator(int input1, int input2){
	if(input1 < input2){
		return input1;
	}
	return input2;
}