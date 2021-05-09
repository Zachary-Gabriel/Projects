#include <cmath>

int fuck_you( int a )
{
	int array[2147483647];
	int i = 0;
	int result = 0;
	int base = 10;
	int base_result =2;

	while (a != 0)
	{
		array[i] = a%base_result;
		a=(a-array[i])/base_result;
		i++;
	}
	for (int j = i-1; j >= 0; j--)
	{
		result+=array[j]*pow(base,j);
	}
	return result;
}