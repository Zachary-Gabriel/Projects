#include "computer.h"
#include "Crescendo.h"

/*	std::string name;
	std::string rounds;
	int length;
*/

Crescendo::Crescendo(){
	name = "Crescendo ";
	rounds = "P S R P S ";
	length = 5;
}

Crescendo::~Crescendo(){

}

std::string Crescendo::getRounds(){
	return rounds;
}