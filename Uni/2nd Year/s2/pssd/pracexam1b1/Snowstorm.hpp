#include <vector>
#include <iostream>
class Snowstorm
{
public:
	int blockedLength (std::vector<int> startpoints, std::vector<int> endpoints)
	{
		int i = 0, j = 0, length = startpoints.size(), result = 0;
		for (i = 0; i < length; i++)
		{
			for (j = 0; j < i; j++)
			{
				if (startpoints[j] < startpoints[i] && endpoints[j] > endpoints[i])
				{
					// std::cout << "1 "<< endpoints[i] - startpoints[i] << std::endl;
					result -= (endpoints[i] - startpoints[i]);
				}
				else if (startpoints[j] < endpoints[i] && endpoints[j] > endpoints[i])
				{
					// std::cout << "2 "<< startpoints[j]- endpoints[i] << std::endl;
					result -= (endpoints[i] - startpoints[j]);
				}
				else if (startpoints[i] > startpoints[j] && startpoints[i] < endpoints[j])
				{
					// std::cout << "3 "<< (startpoints[j]- startpoints[i]) << std::endl;
					result -= (startpoints[i]-startpoints[j]);
				}
				else if (startpoints[i] > startpoints[j] && startpoints[i] < endpoints[j])
				{
					// std::cout << "3 "<< (startpoints[j]- startpoints[i]) << std::endl;
					result -= (startpoints[i]-startpoints[i]);
				}
			}
			result += (endpoints[i] - startpoints[i]);
			// std::cout << result << std::endl;
		}
		return result;
	}	
};
