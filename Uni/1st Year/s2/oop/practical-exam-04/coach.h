#ifndef COACH_H
#define COACH_H
#include "person.h"
#include <string>

class coach: public person{
static int nextID;
	
public:
	coach();
	coach(std::string n, int sl);
	int get_salary();
};

#endif