//uses input from human in computer to output a win - loss record
#include <string>
#include "referee.h"
std::string Referee::ref(std::string hum, std::string comp, std::string raw){
	int length = int(raw[0])-48;
	std::string result = "";
	int i = 0;

	// match fixing
	for(i = 0; i< 2*length; i++){
		if(hum[i]=='R'||hum[i]=='r'){
			if(comp[i]=='R'||comp[i]=='r'){
				result = result +"T ";
			}
			if(comp[i]=='P'||comp[i]=='p'){
				result = result + "W ";
			}
			if(comp[i]=='S'||comp[i]=='s'){
				result = result + "L ";
			}
		}
		if(hum[i]=='P'||hum[i]=='p'){
			if(comp[i]=='R'||comp[i]=='r'){
				result = result + "W ";
			}
			if(comp[i]=='P'||comp[i]=='p'){
				result = result + "T ";
			}
			if(comp[i]=='S'||comp[i]=='s'){
				result = result + "L ";
			}
		}
		if(hum[i]=='S'||hum[i]=='s'){
			if(comp[i]=='R'||comp[i]=='r'){
				result = result + "L ";
			}
			if(comp[i]=='P'||comp[i]=='p'){
				result = result + "W ";
			}
			if(comp[i]=='S'||comp[i]=='s'){
				result = result + "T ";
			}
		}
	}
	return result;
}