#ifndef MAPABSOLUTEVALUE_H
#define MAPABSOLUTEVALUE_H
#include "MapGeneric.h"

class MapAbsoluteValue : public MapGeneric{
	// maps y = x to y = |x|
	int f(int);
};

#endif