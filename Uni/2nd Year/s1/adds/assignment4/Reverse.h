#ifndef REVERSE_H
#define REVERSE_H

#include <string>
class Reverse{
private:
	int BigInt;
	std::string BigString;
public:
	Reverse();
	int reverseDigit(int);
	std::string reverseString(std::string);
	// getters for the variables inside the two reverse functions
	int getBigInt();
	std::string getBigString();
};
#endif