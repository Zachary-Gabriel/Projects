#include <string>
#include "computer.h"
//Creating retard bot that makes rock
std::string Computer::comp(std::string raw){
	int i = 0;
	std::string str2;
	//Uses array length to output the correct number of R's
	int length = int(raw[0])-48;
	for(i = 0; i < length; i++){
		str2 = str2 + "R ";
	}
	return str2;
}