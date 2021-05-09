#include "person.h"
#include "player.h"
#include <string>

Player::Player(std::string n, int sl, int* list, int m){
	name = n;
	service_length = sl;
	occupation = "player";
	NoG = m;
	games = list;
}

int Player::searchGame(int x){
	for(int i = 0; i< NoG; i++){
		if(games[i] == x){
			return i;
		}
	}
	return -1;
}

void Player::set_salary(int s){
	salary = 8000*service_length + s;
}