#include <string>
#include <vector>
#include <iostream>
class RGBStreet
{
public:
	int estimateCost(std::vector<std::string> houses)
	{
		std::string house;
		int length, i, j, k, street = houses.size();
		int costs[street][3], cost_cumsum[street][3], temp, min;

		// weeding the numbers out of the strings
		for (i = 0; i < street; i++) {
			house = houses.at(i);
			length = house.size();
			temp = 0;
			k = 0;
			for (j = 0; j < length; j++) {
				if (house[j] == ' ')
				{
					costs[i][k] = temp;
					cost_cumsum[i][k] = 0;
					temp = 0;
					k++;
				}
				else
				{
					temp = temp*10 + int(house[j]) - 48;
				}
			}
			costs[i][2] = temp;
			cost_cumsum[i][2] = 0;
		}
		// setting the first values of cumsum to reduce seg faults
		for (i = 0; i < 3; i++) cost_cumsum[0][i] = costs[0][i];

		// looping through the arrays
		for (i = 1; i < street; i++)
		{
			for (j = 0; j < 3; j++) // looping through for all three colours
			{
				min = 200001;
				for (k = 0; k < 3; k++) // looping through previous cumsums
				{
					if (j == k) continue; // can't have two houses beside each other that are the same colour
					else
					{
						if (min > cost_cumsum[i-1][k]) // weeding out the smaller cumulative sum of the two sub cumsums
						{
							min = cost_cumsum[i-1][k];
						}
					}
				}
				cost_cumsum[i][j] = min + costs[i][j];
			}
		}

		// picking the smallest one
		min = 200001;
		for (i = 0; i < 3; i++) 
		{
			if (min > cost_cumsum[street-1][i])
			{
				min = cost_cumsum[street-1][i];
			}
		}
		return min;
	}
};