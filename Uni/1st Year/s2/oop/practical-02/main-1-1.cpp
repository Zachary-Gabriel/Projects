#include <cmath>
#include <iostream>
#include <stdlib.h>

extern int diagonal(int array[4][4]);

using namespace std;

int main(void){
	int array[4][4] = {{1,1,1,1,},{1,2,1,1,},{1,3,1,1,},{1,4,1,1,}};
	cout << "diagonal: " << diagonal(array) << endl;
	return 0;
}