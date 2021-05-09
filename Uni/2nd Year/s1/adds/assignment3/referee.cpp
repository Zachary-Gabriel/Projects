#include <string>
#include "referee.h"

std::string Referee::ref(std::string player1, std::string player2, int length){
	std::string result = "";
	int i = 0;

	// match fixing: Wins ans losses revolve around player1
	for(i = 0; i< 2*length; i++){
		if(player1[i]=='R'||player1[i]=='r'){
			if(player2[i]=='R'||player2[i]=='r'){
				result = result +"T ";
			}
			if(player2[i]=='P'||player2[i]=='p'){
				result = result + "L ";
			}
			if(player2[i]=='S'||player2[i]=='s'){
				result = result + "W ";
			}
		}
		if(player1[i]=='P'||player1[i]=='p'){
			if(player2[i]=='R'||player2[i]=='r'){
				result = result + "W ";
			}
			if(player2[i]=='P'||player2[i]=='p'){
				result = result + "T ";
			}
			if(player2[i]=='S'||player2[i]=='s'){
				result = result + "L ";
			}
		}
		if(player1[i]=='S'||player1[i]=='s'){
			if(player2[i]=='R'||player2[i]=='r'){
				result = result + "L ";
			}
			if(player2[i]=='P'||player2[i]=='p'){
				result = result + "W ";
			}
			if(player2[i]=='S'||player2[i]=='s'){
				result = result + "T ";
			}
		}
	}
	return result;
}