#include "team.h"
#include "person.h"
#include "coach.h"
#include "player.h"

//team**
team::team(){
	group = new person*[4];
	coach * c = new coach();
	group[0] = c;
	for (int i =0; i < 3; i++){
		player * p = new player();
		group[i+1] = p;
	}
}

person** team::get_team(){
	return group;
}