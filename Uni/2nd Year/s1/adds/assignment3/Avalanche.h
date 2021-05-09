#ifndef AVALANCHE_H
#define AVALANCHE_H
#include "computer.h"
class Avalanche: public Computer{
public:
	Avalanche();
	~Avalanche();
	std::string getRounds();
};
#endif