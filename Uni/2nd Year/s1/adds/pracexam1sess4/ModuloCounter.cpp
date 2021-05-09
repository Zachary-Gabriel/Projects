

#include "ModuloCounter.h"

#include<iostream>


// put implementation code for the constructor and functions here

ModuloCounter::ModuloCounter(int v, int lim):Counter(v){
	limit = lim;
	if(val>=limit){
		val = val%limit;
	}
}

void ModuloCounter::inc(){
	val++;
	if(val>=limit){
		val = val%limit;
	}
}