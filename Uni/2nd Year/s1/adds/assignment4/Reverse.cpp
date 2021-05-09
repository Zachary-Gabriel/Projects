#include "Reverse.h"
#include <iostream>
Reverse::Reverse(){
	BigInt = 0;
	BigString="";
}
int Reverse::reverseDigit(int value){
	// error checker
	if(value < 0){
		return -1;
	}
	// base case
	if(value < 10){
		BigInt += value;
		return value;
	}
	// iteration
	BigInt = 10*BigInt + 10*(value%10);
	return value%10 + reverseDigit(value/10);
}

std::string Reverse::reverseString(std::string letters){
	// error checker
	if(letters.size() < 1){
		return "ERROR";
	}
	// base case
	if(letters.size() == 1){
		BigString += letters;
		return letters;
	}
	// iteration
	BigString += letters[letters.size()-1];
	return reverseString(letters.substr(0, letters.size()-1));
}
int Reverse::getBigInt(){
	return BigInt;
}
std::string Reverse::getBigString(){
	return BigString;
}