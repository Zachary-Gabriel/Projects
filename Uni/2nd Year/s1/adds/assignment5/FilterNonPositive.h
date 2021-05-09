#ifndef FILTERNONPOSITIVE_H
#define FILTERNONPOSITIVE_H
#include "FilterGeneric.h"

// Returns true if input is non-positive
class FilterNonPositive : public FilterGeneric{
private:
	bool g(int);
};

#endif