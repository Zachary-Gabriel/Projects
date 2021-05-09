#include <string>
#include <iostream>

class JumpyNum
{
	// noot
	bool abs_lt_2 (int a, int b)
	{
		if (a > b)
		{
			if ( a - b < 2 )
			{
				return true ;
			}
			else
			{
				return false ;
			}
		}
		else
		{
			if ( b - a < 2 )
			{
				return true ;
			}
			else
			{
				return false ;
			}
		}
	}

public:
	int howMany(int low, int high)
	{
		using namespace std ;
		int running_total = 0 ;
		int too_long = 0;
		for (int i = low; i <= high; ++i)
		{
			string number = to_string (i) ;
			int oldDigit ;
			int newDigit ;
			bool fuck_it = false ;
			for (int j = 1; j < number.size (); ++j)
			{
				newDigit = int (number[j]) - 48 ;
				oldDigit = int (number[j-1]) - 48 ;
				if ( abs_lt_2 (newDigit, oldDigit) )
				{
					fuck_it = true ;
					break ;
				}
			}
			if (!fuck_it)
			{
				// cout << number << ' ' << endl ;
				++ running_total ;
			}
			too_long++;
			if (too_long == 50000000)
			{
				break;
			}
		}
		return running_total ;
	}
};