#include <string>
#include <iostream>
#include "person.h"
#include "coach.h"
int main (void) {
	Coach c = Coach("name",10);
	Coach d = Coach("name",10);
	
	std::cout << c.get_name() << std::endl;
	c.set_name("chad");
	c.set_salary(0);
	std::cout <<  c.get_name() << std::endl;

	std::cout << c.get_personID() << std::endl;
	std::cout << d.get_personID() << std::endl;	

	std::cout << c.get_occupation() << std::endl;

	std::cout << c.get_salary() << std::endl;

	return 0;
}