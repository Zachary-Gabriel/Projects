#include "computer.h"
#include "Toolbox.h"

/*	std::string name;
	std::string rounds;
	int length;
*/

Toolbox::Toolbox(){
	name = "Toolbox ";
	rounds = "S S S S S ";
	length = 5;
}

std::string Toolbox::getRounds(){
	return rounds;
}