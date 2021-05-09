#ifndef TEAM_H
#define TEAM_H

#include "person.h"
#include "coach.h"
#include "player.h"

class team
{
	person** group;
public:
	team();
	person** get_team();
};

#endif