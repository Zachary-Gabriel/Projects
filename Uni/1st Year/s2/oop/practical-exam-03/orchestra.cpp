#include <string>
#include "musician.h"
#include "orchestra.h"

//	int band_length;
//	int nom;
//	musician * band;

orchestra::orchestra(){
	band_length = 0;
	nom = 0;
	band = new musician[0];
}

orchestra::orchestra(int size){
	nom = 0;
	band_length = size;
	band = new musician[size];
}

int orchestra::get_current_number_of_members(){
	return nom;
}

bool orchestra::has_instrument(std::string instrument){
	for(int i = 0; i < nom; i++){
		if(band[i].get_instrument() == instrument){
			return true;
		}
	}
	return false;
}

musician * orchestra::get_members(){
	return band;
}

bool orchestra::add_musician(musician new_musician){
	if(nom < band_length){
		band[nom] = new_musician;
		nom++;
		return true;
	} else {
		return false;
	}
}

orchestra::~orchestra(){
	delete[] band;
}