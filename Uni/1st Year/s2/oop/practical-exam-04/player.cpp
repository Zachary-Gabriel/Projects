#include "player.h"
#include "person.h"
#include <string>
/*
	std::string name;
	std::string occupation;
protected:
	int salary;
	int serviceLength;
	int personID;
	*/

int player::nextID = 1000;

player::player(){
	set_name("no name");
	set_occupation("player");
	serviceLength = 0;
	personID = nextID;
	nextID++;
	salary = 0;
	games = new int[0];
	nog = 0;
}

player::player(std::string n, int sl, int *list, int m){
	set_name(n);
	set_occupation("player");
	serviceLength = sl;
	personID = nextID;
	nextID++;
	salary = 0;
	games = list;
	nog = m;
	for (int i =0; i < m; i++){
		games[i] = list[i];
	}
}

int player::searchGame(int x){
	for (int i = 0; i < nog; i++){
		if (x == games[i]){
			return i;
		}
	}
	return -1;
}

int player::get_salary(){
	if(serviceLength < 20){
		return salary;
	} else{
		return 3*salary;
	}
}