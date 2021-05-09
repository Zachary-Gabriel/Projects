#include "cart.h"
#include "meerkat.h"
#include <iostream>

// int volume;
// meerkat * animals;

cart::cart(){
	animals = new meerkat[5];
	volume = 0;
}

bool cart::addMeerkat(meerkat cat){
	if(volume < 4){
		animals[volume] = cat;
		volume++;
		return true;
	} else {
		return false;
	}
}

void cart::emptyCart(){
	volume = 0;
	delete[] animals;
	animals = new meerkat[5];
}

void cart::printMeerkats(){
	for (int i = 0; i < volume; i++){
		std::cout << animals[i].getName() << " " << animals[i].getAge() << std::endl;
	}
}

cart::~cart(){
	delete[] animals;
}