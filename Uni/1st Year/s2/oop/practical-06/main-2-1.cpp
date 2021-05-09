#include "meerkat.h"
#include "cart.h"
#include <iostream>

using namespace std;

int main(void) {
	meerkat animal1;
	meerkat animal2;
	meerkat animal3;
	meerkat animal4;
	meerkat animal5;
	
	animal1.setName("bacon");
	animal2.setName("new folder");
	animal3.setName("new folder(5)");
	animal4.setName("jaja and the big z");
	animal5.setName("moses");

	animal1.setAge(80);
	animal2.setAge(8);
	animal3.setAge(2);
	animal4.setAge(1);
	animal5.setAge(0);

	cart yeet;
	bool a1 = yeet.addMeerkat(animal1);
	bool a2 = yeet.addMeerkat(animal2);
	bool a3 = yeet.addMeerkat(animal3);
	bool a4 = yeet.addMeerkat(animal4);
	bool a5 = yeet.addMeerkat(animal5);

	if (a5 == false){
		std::cout << "error" << endl;
	}

	yeet.printMeerkats();

	return 0;
}