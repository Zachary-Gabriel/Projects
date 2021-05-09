#include "FilterForTwoDigitPositive.h"

bool FilterForTwoDigitPositive::g(int input){
	if (input > 9 && input < 100){
		return true;
	}
	return false;
}