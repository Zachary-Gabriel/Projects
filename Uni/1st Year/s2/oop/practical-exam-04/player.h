#ifndef PLAYER_H
#define PLAYER_H
#include "person.h"
#include <string>

class player: public person{
static int nextID;
int nog;
int* games;
	
public:
	player();
	player(std::string n, int sl, int *list, int m);
	int get_salary();
	int searchGame(int x);
};

#endif