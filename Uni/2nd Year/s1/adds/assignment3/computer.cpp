#include "computer.h"

/*	std::string name;
	std::string rounds;
*/

Computer::Computer(){
	// a computer object with these characteristics
	name = "Computer ";
	rounds = "R R R R R ";
	length = 5;
}

int Computer::getLength(std::string player){
	return length;
}

Computer::~Computer(){
}

std::string Computer::getName(){
	return name;
}
