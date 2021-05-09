#include <iostream>
#include <stdlib.h>

extern int sumtwo(int[],int[],int);

int main()
{
	// your code goes here ...
	int arr1[4] = {1,1,1,1};
	int arr2[4] = {1,1,1,2};
	std::cout<< "sum is: "<< sumtwo(arr1, arr2, 4)<<std::endl;
	return 0 ;
}
