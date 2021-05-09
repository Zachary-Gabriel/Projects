#ifndef FILTERODD_H
#define FILTERODD_H
#include "FilterGeneric.h"

// Returns true if odd
class FilterOdd : public FilterGeneric{
private:
	bool g(int);
};

#endif