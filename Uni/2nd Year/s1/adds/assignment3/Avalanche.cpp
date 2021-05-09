#include "computer.h"
#include "Avalanche.h"

/*	std::string name;
	std::string rounds;
	int length;
*/

Avalanche::Avalanche(){
	name = "Avalanche ";
	rounds = "R R R R R ";
	length = 5;
}
Avalanche::~Avalanche(){

}

std::string Avalanche::getRounds(){
	return rounds;
}