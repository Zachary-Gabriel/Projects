#include "Fibonacci.h"

int Fibonacci::function(int input){
	// error checker
	if(input<0){
		return -1;
	}
	//first base case
	
	if(input == 0){
		return 0;
	}
	// second base case
	if(input == 1){
		return 1;
	}
	// recursion
	return function(input-1)+function(input-2);
}