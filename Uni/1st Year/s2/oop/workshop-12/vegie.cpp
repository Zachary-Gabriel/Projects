#include "animal.h"
#include "vegie.h"
#include <string>

 /*
	std::string favourite_food;
	static int nextID;
   */

/*
vegie();
vegie(std::string n, int v);

std::string get_name();
void set_kills(int);
int get_kills();
*/

int vegie::nextID = 100;

vegie::vegie(){
	favourite_food = "grass";
	name= "Safe: ";
	set_volume(0);
	animalID = nextID;
	nextID++;
}

vegie::vegie(std::string n, int v){
	favourite_food = "grass";
	name = n;
	set_volume(v);
	animalID = nextID;
	nextID++;
}

std::string vegie::get_name(){
	return "Safe: " + name;
}

void vegie::set_favourite_food(std::string k){
	favourite_food = k;
}

std::string vegie::get_favourite_food(){
	return favourite_food;
}