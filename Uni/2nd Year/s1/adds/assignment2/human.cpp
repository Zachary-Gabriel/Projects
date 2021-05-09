#include <string>
#include "human.h"
// using std input to create a string to use
std::string Human::hum(std::string raw){
	int i = 0;
	std::string str2 = "";
	int length = int(raw[0])-47;
	//seperating values with spaces
	for(i = 0; i < 2*length; i++){
		if((int(raw[i]) > 64 && int(raw[i]) < 91) || (int(raw[i]) > 96 && int(raw[i]) < 123)){
			str2 = str2 + raw[i] + " ";
		}
	}
	return str2;
}