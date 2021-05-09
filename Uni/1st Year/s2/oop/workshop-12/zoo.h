#ifndef ZOO_H
#define ZOO_H

#include "animal.h"
#include "hunter.h"
#include "vegie.h"
#include <string>

class zoo: public animal{
private:
	std::string name;
	int number_of_animals;
	animal **animals;
public:
	zoo();
	zoo(std::string n,int cows,int lions);
	~zoo();
	std::string get_name();
	int get_number_of_animals();
	animal** get_animals();
};

#endif