#include "person.h"
#include <string>
#include <iostream>

int main(void) 
{
	person * woman  = new person("wife_2",10);
	std::cout << woman->getName() << std::endl;
	delete woman;

	person man = person("jim",60);
	std::cout << man.getName() << std::endl;

	return 0;
}