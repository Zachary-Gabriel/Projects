// function to ...
#include <cmath>


int minimum(int array[], int n)
{
	// your code goes here
	int value = pow(9,99);

	for (int i = 0; i < n; i++){
		if (array[i]>value){
			value = array[i];
		}
	}
	return value
}
