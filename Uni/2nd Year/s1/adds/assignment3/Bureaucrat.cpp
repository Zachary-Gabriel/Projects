#include "computer.h"
#include "Bureaucrat.h"
/*	std::string name;
	std::string rounds;
	int length;
*/

Bureaucrat::Bureaucrat(){
	name = "Bureaucrat ";
	rounds = "P P P P P ";
	length = 5;
}

Bureaucrat::~Bureaucrat(){

}

std::string Bureaucrat::getRounds(){
	return rounds;
}