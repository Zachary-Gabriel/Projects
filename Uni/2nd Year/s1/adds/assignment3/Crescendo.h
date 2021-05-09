#ifndef CRESCENDO_H
#define CRESCENDO_H
#include "computer.h"
class Crescendo: public Computer{
public:
	Crescendo();
	~Crescendo();
	std::string getRounds();
};
#endif