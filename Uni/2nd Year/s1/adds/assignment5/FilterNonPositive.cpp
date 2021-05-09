#include "FilterNonPositive.h"

bool FilterNonPositive::g(int input){
	if(input > 0){
		return false;
	}
	return true;
}