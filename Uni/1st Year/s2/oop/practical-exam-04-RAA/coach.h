#ifndef COACH_H
#define COACH_H
#include "person.h"
#include <string>

class Coach: public Person {

public:
	Coach();
	Coach(std::string n, int s);
	void set_salary(int);
};

#endif