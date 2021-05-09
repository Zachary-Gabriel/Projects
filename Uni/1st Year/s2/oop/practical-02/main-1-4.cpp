#include <cmath>
#include <stdlib.h>
#include <iostream>

extern void print_scaled_matrix(int array[3][3], int scale);

using namespace std;

int main(void){
	int array[3][3] = {{1,2,3},{1,2,3},{1,2,3}};
	print_scaled_matrix(array,3);
	return 0;
}