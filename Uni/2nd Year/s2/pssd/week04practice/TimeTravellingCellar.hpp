#include <vector>
class TimeTravellingCellar
{
public:
	int determineProfit(std::vector<int> profit, std::vector<int> decay)
	{
		size_t i, j, length = profit.size();
		int max = 0, min = 10001, result = 0;
		for (i = 0; i < length; i++)
		{
			for (j = 0; j < length; j ++)
			{
				if (i == j);
				else if ( result < profit.at(i) - decay.at(j))
				{
					result = profit.at(i) - decay.at(j);
				}
			}
		}
		return result;
	}
};