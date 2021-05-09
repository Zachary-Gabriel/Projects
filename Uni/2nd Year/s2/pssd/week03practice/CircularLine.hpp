#include <vector>
#include <iostream>
class CircularLine
{
public:
	int longestTravel(std::vector<int> t)
	{
		int global = 100000, local;
		size_t i, j, length = t.size();
		// going forward
		for (i = 0; i < length; i++)// starting point
		{
			local = 0;
			j = i+1;
			if(j == length)
			{
				j = 0;
			}
			while (i != j)
			{
				j++;
				if (j == length)
				{
					j=-1;
				}
				else
				{
					local += t.at(j);
				}
			}
			if (global > local)
			{
				global = local;
			}
		}
		// going backward
		for (i = 0; i < length; i++)// starting point
		{
			local = 0;
			j = i-1;
			if(j == -1)
			{
				j = 0;
			}
			while (i+1 != j)
			{
				j--;
				if (j == -1)
				{
					j = length;
				}
				else
				{
					local += t.at(j);
				}
			}
			if (global > local)
			{
				global = local;
			}
		}
		return global;
	}
};
