#include "zoo.h"
#include "animal.h"
#include "vegie.h"
#include "hunter.h"



#include <string>

/*
std::string name;
int number_of_animals;
animal **animals;
*/

zoo::zoo(){
	name = "basic bitch zoo";
	int number_of_animals = 0;
	animals = new animal*[0];
}

zoo::zoo(std::string n,int cows,int lions){
	name = n;
	number_of_animals = cows + lions;
	animals = new animal*[number_of_animals];

	for(int i = cows; i < number_of_animals; i++){
		hunter* Clarence = new hunter("Clarence", 50);
		animals[i] = Clarence;
	}
	for (int i = 0; i< cows; i++){
		vegie* Daisy = new vegie("Daisy", 100);
		animals[i] = Daisy;
	}
	
}

zoo::~zoo(){
	delete[] animals;
}

std::string zoo::get_name(){
	return name;
}

int zoo::get_number_of_animals(){
	return number_of_animals;
}

animal** zoo::get_animals(){
	return animals;
}