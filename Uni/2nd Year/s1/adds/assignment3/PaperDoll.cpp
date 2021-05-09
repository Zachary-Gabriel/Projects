#include "computer.h"
#include "PaperDoll.h"

/*	std::string name;
	std::string rounds;
	int length;
*/

PaperDoll::PaperDoll(){
	name = "PaperDoll ";
	rounds = "P S S P S ";
	length = 5;
}

PaperDoll::~PaperDoll(){

}

std::string PaperDoll::getRounds(){
	return rounds;
}