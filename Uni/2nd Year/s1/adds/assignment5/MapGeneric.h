#ifndef MAPGENERIC_H
#define MAPGENERIC_H
#include <vector>

class MapGeneric{
private:
	// maps y = x to y = f(x)
	virtual int f(int) = 0;
	std::vector<int> output;
public:
	// applies to function f to the input vector
	std::vector<int> map(std::vector<int>);
};

#endif