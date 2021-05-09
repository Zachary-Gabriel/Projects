#include <string>
#include <iostream>
#include "musician.h"
#include "orchestra.h"

int main (void)
{
	musician bard = musician("voice",100);
	musician penist = musician("piano",100);
	musician guitarist = musician("guitar",100);
	musician drummer = musician("drums",100);
	musician singer = musician("voice",100);
	int size = 4;

	orchestra not_a_choir = orchestra(size);
	bool yes1 = not_a_choir.add_musician(bard);
	bool yes2 = not_a_choir.add_musician(penist);
	bool yes3 = not_a_choir.add_musician(guitarist);
	bool yes4 = not_a_choir.add_musician(drummer);
	bool no1 = not_a_choir.add_musician(singer);

	std::cout << yes1 << " " << yes2 << " " << yes3 << " " << yes4 << " " << no1 << " " << std::endl;

	musician * people = not_a_choir.get_members();

	for (int i = 0; i< size; i++){
		std::cout << "member: " << people[i].get_instrument() << std::endl;
	}

	return 0;
}