#include <iostream>
#include "human.h"
#include "referee.h"
#include "computer.h"

int main(void){
	//running my code
	std::string str1;
	std::getline(std::cin,str1);
	std::string str2 = Human::hum(str1);
	std::string str3 = Computer::comp(str1);
	std::string str4 = Referee::ref(str2,str3,str1);
	std::cout << str4 << std::endl;
	return 0;
}