#include <vector>
#include <algorithm>
#include <iostream>
class SimpleDuplicateRemover
{
public:
	std::vector<int> process(std::vector<int> sequence)
	{
		//allocating memory
		std::vector<int> result;
		int arr[1001], value, i, length = sequence.size();

		// setting everything to zero
		for (i = 1; i < 1001; i++) arr[i] = 0;
		// adding the elements to list back to front, and only once
		for (i = length-1; i >= 0; i--)
		{
			value = sequence.at(i);
			if (arr[value] == 0)
			{
				result.push_back(value);
				arr[value] = 1;
			}
		}
		std::reverse(result.begin(),result.end());
		return result;
	}
};