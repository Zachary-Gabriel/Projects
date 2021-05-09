#include <vector>
class ChickensAndCows
{
public:
	std::vector<int> howMany (int heads, int legs)
{
	int cows = 0;
	int chickens =0;
	std::vector<int> result;
	while (cows < heads)
	{
		if (legs%2 == 0 && legs%(heads-cows) == 0)
		{

			if (legs/(heads-cows) == 2)
			{
				chickens = heads - cows;
				break;
			}
		}
		legs -= 4;
		cows++;
	}
	if (cows >= heads){
		return result;
	}

	result.push_back (chickens);
	result.push_back (cows);
	return result;
}	
};