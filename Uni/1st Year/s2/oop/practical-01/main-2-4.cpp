#include <iostream>
#include <stdlib.h>

extern bool ascending(int*,int);

int main(int argc,char **argv)
{
	// your code goes here ...
	int arr[4]= {1,2,3,4};
	std::cout<< "Is ascending?: "<< ascending(arr, 4)<<std::endl;
	int arr1[4]= {1,2,3,1};
	std::cout<< "Is ascending?: "<< ascending(arr1, 4)<<std::endl;
	return 0 ;
}