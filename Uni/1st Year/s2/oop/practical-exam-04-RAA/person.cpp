#include <string>
#include "person.h"

int Person::nextID = 0;

Person::Person(){
	service_length = 0;
	personID = nextID;
	nextID++;
	name = "";
	occupation = "";
	salary = 0;
}

Person::Person(std::string n, std::string o, int sl){
	service_length = sl;
	personID = nextID;
	nextID++;
	name = n;
	occupation = o;
	salary = 0;
}

//getters
int Person::get_personID(){
	return personID;
}

std::string Person::get_name(){
	return name;
}

std::string Person::get_occupation(){
	return occupation;
}

int Person::get_salary(){
	return salary;
}

//setters

void Person::set_name(std::string n){
	name = n;
}
void Person::set_occupation(std::string o){
	occupation = o;
}
