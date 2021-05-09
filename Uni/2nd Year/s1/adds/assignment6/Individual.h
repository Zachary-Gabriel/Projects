#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <string>

class Individual{
private:
	std::string binaryString;
public:
	// Constructors
	Individual(int);
	Individual(std::string);

	// Getters
	std::string getString();
	int getBit(int);
	int getLength();
	// Flips bit from zero to one or one to zero.
	void flipBit(int);
	// Returns the longest string of ones in a row.
	int getMaxOnes();
};

#endif