#include <string>
#include "musician.h"

/*  variables:
	- std::string inst; represents the instrument the object currently has
	- int exp; represents the experience the object currently has
 */

musician::musician(void){
	//starts with nothing
	exp = 0;
	inst = "?";
}

musician::musician(std::string instrument, int experience){
	//starts with expected inputs
	exp = experience;
	inst = instrument;
}

std::string musician::get_instrument(void){
	return inst;
}

int musician::get_experience(void){
	return exp;
}

musician::~musician(void){
	// no "new"s created in class
}