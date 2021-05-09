#ifndef HUNTER_H
#define HUNTER_H

#include "animal.h"
#include <string>

class hunter: public animal{
private:
	int kills;
	static int nextID;
public:
	hunter();
	hunter(std::string n, int v);

	std::string get_name();
	void set_kills(int);
	int get_kills();
};
#endif