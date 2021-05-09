#ifndef VEGIE_H
#define VEGIE_H

#include "animal.h"
#include <string>

class vegie: public animal{
private:
	std::string favourite_food;
	static int nextID;
public:
	vegie();
	vegie(std::string n, int v);

	std::string get_name();
	void set_favourite_food(std::string);
	std::string get_favourite_food();
};
#endif