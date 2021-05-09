#include "computer.h"
#include "RandomComputer.h"
#include <cmath>

/*	std::string name;
	std::string rounds;
	int length;
*/

RandomComputer::RandomComputer(){
	name = "RandomComputer ";
	length = 5;
	//returns r p or s then adds to existing string for 5 rounds
	std::string rounds = "";
	for(int i = 0; i < length; i++){
		int temp = rand() % 3;
		if(temp == 0){
			rounds = rounds + "R ";
		} if(temp == 1){
			rounds = rounds + "P ";
		} else{
			rounds = rounds + "S ";
		}
	}
}

std::string RandomComputer::getRounds(){
	return rounds;
}
