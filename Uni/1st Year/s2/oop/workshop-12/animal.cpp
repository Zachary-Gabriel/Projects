#include "animal.h"
#include <string>

/*
Object description:
int volume;
std::string name;
int animalID;
*/

animal::animal(){
	volume = 0;
	name = " no name";
	animalID = 0;
}

animal::animal(std::string n, int v){
	volume = v;
	name = n;
	animalID = 0;
}

animal::~animal(){
}

int animal::get_volume(){
	return volume;
}

int animal::get_animalID(){
	return animalID;
}

void animal::set_volume(int v){
	volume = v;
}

void animal::set_name(std::string n){
	name = n;
}