#include <iostream>
#include <stdlib.h>

extern int sum_array(int[], int);

int main()
{
	// your code goes here ...
	int arr[4]= {1,2,3,4};
	std::cout<< "sum is: "<< sum_array(arr, 4)<<std::endl;
}