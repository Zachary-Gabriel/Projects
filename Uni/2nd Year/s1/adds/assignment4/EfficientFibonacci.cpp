#include "EfficientFibonacci.h"

int EfficientFibonacci::function(int input, int* array){
	//checks if calculation has already been done
	if(array[input-1]!=0){
		return array[input-1];
	}
	//error check
	if(input<0){
		return -1;
	}
	//base case
	if(input == 0){
		return 0;
	}
	//base case
	if(input == 1){
		return 1;
	}
	//recursion
	array[input-1] = function(input-1, array)+function(input-2, array);
	return array[input-1];
}

int EfficientFibonacci::driver(int input){
	// array for storing used data;
	int *array = new int[input];
	for(int i = 0; i < input; i++){
		array[i]=0;
	}
	// does recursion
	int temp = function(input, array);
	return temp;
}