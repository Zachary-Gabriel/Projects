#ifndef PAPERDOLL_H
#define PAPERDOLL_H
#include "computer.h"
class PaperDoll: public Computer{
public:
	PaperDoll();
	~PaperDoll();
	std::string getRounds();
};
#endif