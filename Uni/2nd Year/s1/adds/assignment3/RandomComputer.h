#ifndef RANDOMCOMPUTER_H
#define RANDOMCOMPUTER_H
#include "computer.h"
class RandomComputer: public Computer{
public:
	RandomComputer();
	std::string getRounds();
};
#endif