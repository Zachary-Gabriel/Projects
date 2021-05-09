#include "animal.h"
#include "hunter.h"
#include <string>

 /*
	Description:
	int kills;
	std::string name;
	int animalID;
	static int nextID;
   */
int hunter::nextID = 1000;

hunter::hunter(){
	kills = 0;
	name= "Hunter: ";
	set_volume(0);
	animalID = nextID;
	nextID++;
}

hunter::hunter(std::string n, int v){
	kills = 0;
	name = n;
	set_volume(v);
	animalID = nextID;
	nextID++;
}

std::string hunter::get_name(){
	return "Hunter: " + name;
}

void hunter::set_kills(int k){
	kills = k;
}

int hunter::get_kills(){
	return kills;
}