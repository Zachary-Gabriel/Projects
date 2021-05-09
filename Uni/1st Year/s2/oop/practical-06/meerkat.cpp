#include <string>
#include "meerkat.h"

// consrtuctors
meerkat::meerkat(void){
	age;
	name;
}

meerkat::meerkat(std::string Name,int Age){
	age = Age;
	name = Name;
}

// getters
std::string meerkat::getName(void){
	return name;
}

int meerkat::getAge(void){
	return age;
}

// setters
void meerkat::setName(std::string meerName){
	name = meerName;
}

void meerkat::setAge(int meerAge){
	age = meerAge;
}

// destructors
meerkat::~meerkat(void){
	//no 'new's
}