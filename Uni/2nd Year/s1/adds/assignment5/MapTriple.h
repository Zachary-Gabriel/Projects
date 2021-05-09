#ifndef MAPTRIPLE_H
#define MAPTRIPLE_H
#include "MapGeneric.h"

class MapTriple : public MapGeneric{
private:
	// maps from y = x to y = 3x.
	int f(int);
};

#endif