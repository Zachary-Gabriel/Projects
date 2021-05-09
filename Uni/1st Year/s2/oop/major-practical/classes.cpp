#include "classes.h"

// Constructors
classes::classes(){
	health = 0;
	attack = 0;
	turns = 0;
	agility = 0;
	luck = 0;
}

classes::classes(int hp, int att, int turn, int agil, int luc){
	health = hp;
	attack = att;
	turns = turn;
	agility = agil;
	luck = luc;
}

// Getters
int classes::get_health(){
	return health;
}

int classes::get_attack(){
	return attack;
}

int classes::get_turns(){
	return turns;
}

int classes::get_agility(){
	return agility;
}

int classes::get_luck(){
	return luck;
}

// Setters
void classes::set_health(int new_health){
	health = new_health;
}

void classes::set_attack(int new_attack){
	attack = new_attack;
}

void classes::set_turns(int new_turns){
	turns = new_turns;
}

void classes::set_agility(int new_agility){
	agility = new_agility;
}

void classes::set_luck(int new_luck){
	luck = new_luck;
}

// Destructors
classes::~classes(){
	//no arrays
}