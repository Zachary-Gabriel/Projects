#ifndef TOOLBOX_H
#define TOOLBOX_H
#include "computer.h"
class Toolbox: public Computer{
public:
	Toolbox();
	std::string getRounds();
};
#endif