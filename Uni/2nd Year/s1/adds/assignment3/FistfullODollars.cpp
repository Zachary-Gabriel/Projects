#include "computer.h"
#include "FistfullODollars.h"

/*	std::string name;
	std::string rounds;
	int length;
*/

FistfullODollars::FistfullODollars(){
	name = "FistfullODollars ";
	rounds = "R P P R P ";
	length = 5;
}

FistfullODollars::~FistfullODollars(){

}

std::string FistfullODollars::getRounds(){
	return rounds;
}