#include "Palindrome.h"
#include <iostream>
#include <stdio.h>
#include <string>

int main(void){
	//input a string
	std::string str1 = "";
	std::getline (std::cin,str1);
	
	//run my code
	str1 = Palindrome::removeNonLetters(str1);
	std::cout << str1 << std::endl;

	return 0;
}