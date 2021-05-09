// function to ...
#include <cmath>
bool ascending(int array[], int n)
{
	// your code goes here
	float lowest= -pow(1,99);

	for (int i = 0; i < n; i++){
		
		if (lowest > array[i]){
			return false;
		}

		lowest = array[i];
	}
	return true;
}
