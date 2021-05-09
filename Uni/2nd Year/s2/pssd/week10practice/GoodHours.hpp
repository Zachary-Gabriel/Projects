#include <string>
#include <iostream>

class GoodHours
{
	//noot
	bool is_good (std::string time)
	{
		using namespace std ;
		int d1 = int (time[0]) - 48 ;
		int d2 = int (time[1]) - 48 ;
		int d3 = int (time[3]) - 48 ;
		int d4 = int (time[4]) - 48 ;

		if (d1*d2*d3 == d4 || d1*d2 == d3*d4 || d1 == d2*d3*d4)
		{
			return true ;
		}
		return false ;
	}
	std::string increment (std::string time)
	{
		using namespace std ;
		int d1 = int (time[0]) - 48 ;
		int d2 = int (time[1]) - 48 ;
		int d3 = int (time[3]) - 48 ;
		int d4 = int (time[4]) - 48 ;

		++ d4 ;
		if ( d4 == 10 )
		{
			d4 = 0 ;
			++ d3 ;
			if ( d3 == 6 )
			{
				d3 = 0 ;
				++ d2 ;
				if ( d2 == 4 && d1 == 2 )
				{
					d1 = 0 ;
					d2 = 0 ;
				}
				else if ( d2 == 10 )
				{
					d2 = 0 ;
					++ d1 ;
				}
			}
		}

		string c1 = to_string (d1) ;
		string c2 = to_string (d2) ;
		string c3 = to_string (d3) ;
		string c4 = to_string (d4) ;

		string time2 = c1 + c2 + ":" + c3 + c4 ;
		return time2 ;
	}
public:
	int howMany(std::string beforeTime, std::string afterTime)
	{
		using namespace std ;

		int result = 0;
		
		while (afterTime != beforeTime)
		{
			if (is_good (beforeTime) )
			{
				// cout << beforeTime << endl ;
				++ result ;
			}
			beforeTime = increment (beforeTime) ;
		}

		if ( is_good (beforeTime) )
		{
			// cout << beforeTime << endl ;
			++ result ;
		}

		return result ;
	}
};