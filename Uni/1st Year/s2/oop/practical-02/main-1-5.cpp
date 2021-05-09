//Prac 02 1.5
#include <cmath>
#include <stdlib.h>
#include <iostream>

extern void print_summed_matrices(int array1[3][3],int array2[3][3]);


using namespace std;

int main(void) {
	int matrix1[3][3] = {{0,1,2},{3,4,5},{6,7,8}} ;
	int matrix2[3][3] = {{0,0,0},{2,2,2},{-5,-4,8}} ;
	print_summed_matrices(matrix1,matrix2) ;
	return 0;
}