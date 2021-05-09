#ifndef FILTERGENERIC_H
#define FILTERGENERIC_H
#include <vector>

// Defines pure virtual function "g" and function "filter" so g the vector can use the g function
class FilterGeneric{
private:
	std::vector<int> output;
	virtual bool g(int) = 0;
public:
	std::vector<int> filter(std::vector<int>);
};

#endif