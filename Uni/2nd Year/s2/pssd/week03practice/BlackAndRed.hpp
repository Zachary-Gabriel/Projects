#include <string>
#include <iostream>
class BlackAndRed
{
public:
	int cut(std::string deck)
	{
		std::string yeet;
		int count = 0, index=90000;
		bool gg = false;
		size_t j=0, i = 0, length = deck.size();

		for ( j = 0; j < length; j++)
		{
			gg= false;
			yeet = deck.substr(j) + deck.substr(0,j);
			for (i = 0; i < length; i++)
			{
				if (yeet[i] == 'B')
				{
					count = count+1;
				}
				if (yeet[i] == 'R')
				{
					count= count -1;
				}
				if (count < 0)
				{
					gg = true;
				}
			}
			if (gg == false && j < index)
			{
				index = j;
			}
		}
		return index;
	}
};