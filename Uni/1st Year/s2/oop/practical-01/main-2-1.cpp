#include <iostream>
#include <stdlib.h>

extern int minimum(int*,int);

int main()
{
	// your code goes here ...
	int arr[4]= {1,2,7,4};
	std::cout<< "Min: "<< minimum(arr, 4)<<std::endl;
	return 0;
}
