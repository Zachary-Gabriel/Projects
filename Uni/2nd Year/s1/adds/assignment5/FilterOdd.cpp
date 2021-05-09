#include "FilterOdd.h"

bool FilterOdd::g(int input){
	if(input%2 == 0){
		return false;
	}
	return true;
}