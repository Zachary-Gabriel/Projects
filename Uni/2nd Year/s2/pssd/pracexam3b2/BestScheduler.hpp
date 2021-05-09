#include "WordGame.hpp"
#include <vector>
#include <string>
#include <iostream>


class BestScheduler
{
public:
	int minTime (std::vector<int> thyme, std::vector<std::string> prec)
	{
		using namespace std;
		int timeseg = 0;
		int result = 0;
		int done[thyme.size()] ;
		for (int i = 0; i < thyme.size();++i)
		{
			done[i] = 0;
		}
		while ( timeseg < thyme.size ())
		{
			int lclMax = 0;
			for (int i = 0 ; i < thyme.size() ; ++i)
			{
				for (int j = 0; j < prec.size (); ++j)
				{
					string row = pre.at(j);
					if (row[i] == 'N')
					{
						if (lclMax < thyme[i])
						{
							lclMax = thyme[i];
							done[i] = 1;
						}
					}

				}
			}
			result+= lclMax;
			timeseg++;
		}
		return result;
	}
};