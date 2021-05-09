#include "EfficientFibonacci.h"
#include "Fibonacci.h"
#include "Reverse.h"
#include <iostream>
#include <sstream>


int main(){
	//setting up input and breaking into variables using stringstream
	std::string input ="";
	getline(std::cin, input);
	std::stringstream ss;
	std::string reverseDigitString;
	std::string stringInput;
	std::string fib1;
	std::string fib2;
	ss << input;
	ss >> reverseDigitString >> stringInput >> fib1 >> fib2;

	//preallocating variables in memory
	Reverse reverseObject = Reverse();
	EfficientFibonacci fibObject2 = EfficientFibonacci();
	Fibonacci fibObject = Fibonacci();
	bool noProblems = true;
	int i = 0;
	int reverseInt = 0;

	// reverse digit	
	// checking to see if int
	for(i = 0; i < reverseDigitString.size(); i++){
		if(int(reverseDigitString[i]) > 57 || int(reverseDigitString[i]) < 48){
			noProblems = false;
		}
	}
	if(noProblems == false){
		reverseDigitString = "ERROR";
	} else {
		//running swap function for number
		reverseInt = std::stoi(reverseDigitString);
		int temp = reverseObject.reverseDigit(reverseInt);
		reverseDigitString = std::to_string(reverseObject.getBigInt());
	}

	//reverse string
	std::string temp = reverseObject.reverseString(stringInput);
	stringInput = reverseObject.getBigString();

	// inefficient fibanacci
	// checking to see if int
	noProblems = true;
	for(i = 0; i<fib1.size(); i++){
		if(int(fib1[i]) > 57 || int(fib1[i]) < 48){
			noProblems = false;
		}
	}

	if(noProblems == false){
		fib1 = "ERROR";
	} else {
		//running fib function
		int tempFib1 = std::stoi(fib1);
		int temp = fibObject.function(tempFib1);
		fib1 = std::to_string(temp);
	}

	// efficient fibanacci
	// checking to see if int
	noProblems = true;
	for(i = 0; i < fib2.size(); i++){
		if(int(fib2[i]) > 57 || int(fib2[i]) < 48){
			noProblems = false;
		}
	}

	if(noProblems == false){
		fib2 = "ERROR";
	} else {
		//running fib function
		int tempFib2 = std::stoi(fib2);
		int temp = fibObject2.driver(tempFib2);
		fib2 = std::to_string(temp);
	}

	// output:
	std::cout << reverseDigitString << " " << stringInput << " " << fib1 << " " << fib2 << std::endl;
	return 0;
}