#include <string>
#include <vector>
class VeryInterestingMovie
{
public:
	int maximumPupils (std::vector<std::string> seats)
	{
		int sum = 0, cum1 = 0, cum2 = 0;
		for (int i = 0 ; i < seats.size(); ++i)
		{
			std::string row = seats.at (i);
			for (int j = 0; j < row.size(); ++j)
			{
				if (j%2 == 0)
				{
					if (row[j] == 'Y')
					{
						++cum1 ;
					}
					if (row[j] == 'N')
					{
						if (cum1 < cum2) sum += cum2 ;
						else sum += cum1 ;
						cum1 = 0;
						cum2 = 0;
					}
				}
				else
				{
					if (row[j] == 'Y')
					{
						++cum2 ;
					}
					if (row[j] == 'N')
					{
						if (cum1 < cum2) sum += cum2 ;
						else sum += cum1 ;
						cum1 = 0;
						cum2 = 0;
					}
				}
			}
			if (cum1 < cum2) sum += cum2 ;
			else sum += cum1 ;
			cum1 = 0;
			cum2 = 0;
		}
		return sum;
	}
} ;