#include <stdlib.h>
#include <cmath>
#include <iostream>

extern int binary_to_number(int binary_digets[], int number_of_digets);

using namespace std;

int main(void){
	int array[4] = {1,0,1,0};
	cout << "1010 " << binary_to_number(array,4) << endl;
	return 0;
}