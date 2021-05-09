#ifndef MAPSQUARE_H
#define MAPSQUARE_H
#include "MapGeneric.h"

class MapSquare : public MapGeneric{
	// maps y = x to y = x^2
	int f(int);
};

#endif