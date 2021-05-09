#ifndef BUREAUCRAT_H
#define BUREAUCRAT_H
#include "computer.h"
class Bureaucrat: public Computer{
public:
	Bureaucrat();
	~Bureaucrat();
	std::string getRounds();
};
#endif