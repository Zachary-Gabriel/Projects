#include <cmath>
#include <stdlib.h>
#include <iostream>

extern void count_numbers(int array[4][4]);

using namespace std;

int main(void){
	int array[4][4] = {{1,2,3,4},{5,6,7,8},{9,0,1,2},{1,2,3,4}};
	count_numbers(array);
	return 0;
}