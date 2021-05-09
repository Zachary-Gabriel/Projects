#include <vector>

class ElevatorLimit
{
public:
	std::vector<int> getRange(std::vector<int> enter, std::vector<int> exit, int physicalLimit)
	{
		// Preallocating memory
		std::vector<int> result;
		int floors = enter.size(), max = physicalLimit, i = 0, min = 0, ppl = 0;
		for (i = 0; i < floors; i++)
		{
			// looking for largest negative number
			ppl -= exit[i];
			if (min +ppl <= 0)
			{
				min = -1*ppl;
			}

			// looking for largest positive number
			ppl +=enter[i];
			if (max > physicalLimit - ppl)
			{
				max = physicalLimit - ppl;
			}

			// too many people
			if (ppl > physicalLimit || min > physicalLimit || max < 0)
			{
				return result;
			}
			
		}
		// Returning result
		if (min > max) 
		{
			return result;
		}

		result.push_back(min);
		result.push_back(max);
		return result;
	}
};