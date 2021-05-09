#ifndef RECUDEGCD_H
#define RECUDEGCD_H
#include "ReduceGeneric.h"

// defines Binary Operatos to find the GCD of two inputs
class ReduceGCD: public ReduceGeneric{
private:
	// GCD of two numbers.
	int binaryOperator(int, int);
};

#endif