#include "person.h"
#include <iostream>
#include "coach.h"

int main(){
	coach feetus = coach("john",20);
	std::cout << feetus.get_salary() << std::endl; 
	return 0;
}