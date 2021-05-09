#include <iostream>
#include <cmath>
#include <vector>

class RugSizes
{
public:
	int rugCount (int area)
	{
		int i, results = 0, temp;
		for (i = 1; i <= area; i++)
		{
			if (area%i == 0)
			{
				temp = area/i;
				if (temp == i)
				{
					results+=2;
				}
				if (temp%2 == 0 && i%2 == 0)
				{
					continue;
				} 
				else
				{
					results++;
				}
			}
		}
		return results/2;
	}
};