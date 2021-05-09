#ifndef ORCHESTRA_H
#define ORCHESTRA_H

#include "musician.h"
#include <string>

class orchestra {
	int band_length;
	int nom; // Number Of Memebers;
	musician * band;

public:
	orchestra();
	orchestra(int size);

	int get_current_number_of_members();
	bool has_instrument(std::string instrument);
	musician * get_members();
	bool add_musician(musician new_musician);

	~orchestra();
};
#endif