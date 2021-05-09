#ifndef CLASSES_H
#define CLASSES_H

class classes
{
	int health;
	int attack;
	int turns;
	int agility;
	int luck;

public:
	// Constructer
	classes();
	classes(int hp, int att, int turn, int agil, int luc);

	// Getters
	int get_health();
	int get_attack();
	int get_turns();
	int get_agility();
	int get_luck();

	// Setters
	void set_health(int new_health);
	void set_attack(int new_attack);
	void set_turns(int new_turns);
	void set_agility(int new_agility);
	void set_luck(int new_luck);

	// Destructors
	~classes();
};
#endif