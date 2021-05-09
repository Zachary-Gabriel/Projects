#include "meerkat.h"
#include <iostream>

using namespace std;

int main(void) {
	meerkat animal1;
	meerkat animal2;
	meerkat animal3;
	meerkat animal4;

	cout << animal4.getName() << endl;
	
	animal1.setName("bacon");
	animal2.setName("new folder");
	animal3.setName("new folder(5)");
	animal4.setName("jaja and the big z");

	animal1.setAge(80);
	animal2.setAge(8);
	animal3.setAge(2);
	animal4.setAge(1);

	cout << animal3.getName() << endl;

	return 0;
}