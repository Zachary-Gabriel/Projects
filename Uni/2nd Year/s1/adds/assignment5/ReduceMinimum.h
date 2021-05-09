#ifndef REDUCEMIN_H
#define REDUCEMIN_H
#include "ReduceGeneric.h"

class ReduceMinimum : public ReduceGeneric{
private:
	// Defines Binary operator to find the minimum value between its two inputs
	int binaryOperator(int, int);
};

#endif