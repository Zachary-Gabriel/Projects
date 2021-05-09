#include <string>
#include <vector>
#include <iostream>
#include <limits>
class QuickSums
{
	int recursion (std::vector<int> numbers, int sum)
	{
		int min = 1000000, thisPivot = 0, thisNumber = 0, temp, multiplyBy = 10;
		size_t i = 0, j = 0, length = numbers.size();
		std::vector<int> v;
		if (length >= 2)
		{
			// checking if we have a good sum
			for (i = 0; i < length; i++)
			{
				thisNumber += v[i];
			}
			if (thisNumber == sum)
			{
				return 0;
			}
			// iterating for bad sum
			for (i = 0; i < length-1; i++)
			{
				thisPivot = numbers.at(i);
				multiplyBy = 10;
				
				for (j = 0; j < length; j++) // making a whole heap of subvectors with different merged indicies
				{
					thisNumber = numbers.at(j);
					if (i == j) // reducing vector size by 1
					{
					}
					else if (i+1 == j) // merging two numbers
					{
						while (thisNumber % multiplyBy != thisNumber)
						{
							multiplyBy = 10 * multiplyBy;
						}
						v.push_back(multiplyBy*thisPivot + thisNumber);
					}
					else // adding the rest of the numbers
					{
						v.push_back(thisNumber);
					}
				}

				// finding the smallest one				
				temp = recursion (v, sum);
				if (temp != -1 && min > temp){
					min = 1 + temp;
				}
				// freeing memory
				//v.clear();
				// v.shrink_to_fit();
			}

			// if there isnt a combination that worked
			if (min == 1000000)
			{
				return -1;
			}
			// if there is
			return min;
		}
		else if (length == 1)	// ending recursive loop
		{
			thisPivot = numbers.at(0);
			if (thisPivot != sum)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return -1;
		}
	}
public:
	int minSums(std::string numbers, int sum)
	{
		std::vector<int> v;
		unsigned int length = numbers.size();
		int max;
		for (unsigned i = 0; i < length; i++)
		{
			max = int(numbers[i])-48;
			v.push_back(max);
		}
		max = recursion (v, sum);

		if (max == -1)
		{
			return -1;
		}
		else
		{
			return length-max;
		}
	}
};
