#include <vector>
#include <iostream>

class BigBurger
{
public:
	int maxWait(std::vector<int> arrival, std::vector<int> service)
	{
		using namespace std;
		int time[12000] ;
		int excess = 0;
		int result = 0;
		int arr = arrival.at(0);
		int ser = service.at(0);
		int tempExcess = 0;
		bool add_it = false ;

		for (int i = 0; i < 12000;  ++i)
		{
			time[i] = 0;
		}
		for (int i = 0 ; i < arrival.size(); ++i)
		{
			arr = arrival.at(i);
			ser = service.at(i);
			tempExcess = 0;
			for (int j = arr; j < ser+arr; ++j)
			{
				if (time[j] == 1)
				{
					if (j == arr)
					{
						add_it = true;
					}
					++tempExcess ;
					if (add_it)
					{
						// cout << j << ' ';
						++excess ;
					}
				}
				
				if (time[j] == 0)
				{
					add_it = false ;
				}				
				time[j] = 1;
			}
			int j = ser+arr;
			while (j <  ser+arr+tempExcess)
			{
				if (time[j] == 1)
				{
					tempExcess++;
					excess++;
				}
				time[j] = 1;
				++j;
			}
			if (result < excess)
			{
				result = excess ;
			}
			excess = 0;
		}
		// cout << endl ;
		return result ;
	}
};