#include <string>
#include <iostream>
#include "musician.h"

int main (void)
{
	musician bard = musician("guitar",100);
	std::cout << "Bard's instrument: " << bard.get_instrument() << " and experience :" << bard.get_experience() << std::endl;
	return 0;
}