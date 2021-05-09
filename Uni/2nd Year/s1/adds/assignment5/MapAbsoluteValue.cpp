#include "MapAbsoluteValue.h"

int MapAbsoluteValue::f(int input){
	if(input<0){
		return -input;
	}
	return input;
}