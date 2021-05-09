#ifndef MUSICIAN_H
#define MUSICIAN_H

#include <string>

class musician 
{
	// private variables
	std::string inst;
	int exp;
public:
	// public functions
	musician();
	musician(std::string instrument, int experience);

	std::string get_instrument();
	int get_experience();

	~musician();
};

#endif