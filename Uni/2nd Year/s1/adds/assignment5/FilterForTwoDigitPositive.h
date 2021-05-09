#ifndef FILTERFORTWODIGITPOSITIVE_H
#define FILTERFORTWODIGITPOSITIVE_H
#include "FilterGeneric.h"

// Returns true if input is between 10 and 99
class FilterForTwoDigitPositive : public FilterGeneric{
private:
	// Defines the function "g" for FilterGeneric
	bool g(int);
};

#endif