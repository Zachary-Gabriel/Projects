#ifndef PLAYER_H
#define PLAYER_H
#include "person.h"

class Player : public Person{

int* games;
int NoG;

public:
	void set_salary(int);
	Player(std::string n, int sl, int* list, int m);
	int searchGame(int x);
};
#endif