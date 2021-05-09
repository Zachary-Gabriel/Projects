#include "person.h"
#include <string>
person::person(){
	name;
	salary;
}

person::person(std::string myName,int Salary){
	name = myName;
	salary = Salary;
}

void person::setName(std::string myName){
	name = myName;
}

std::string person::getName(){
	return name;
}

void person::setSalary(int mySalary){
	salary = mySalary;
}

int person::getSalary(){
	return salary;
}

person::~person(){
	//nothing
}