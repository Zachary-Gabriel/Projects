#include "person.h"
#include <string>

person::person(){
	name = "no name";
	occupation = "no job";
	salary = 0;
	serviceLength = 0;
	personID = 0;
}

person::person(std::string n, std::string o, int sl){
	name = n;
	occupation = o;
	salary = 0;
	serviceLength = sl;
	personID = 0;
}

//setters
void person::set_name(std::string n){
	name = n;
}

void person::set_occupation(std::string o){
	occupation = o;
}

void person::set_salary(int sl){
	salary = sl;
}

	//getters
std::string person::get_name(){
	return name;
}

std::string person::get_occupation(){
	return occupation;
}

int person::get_personID(){
	return personID;
}