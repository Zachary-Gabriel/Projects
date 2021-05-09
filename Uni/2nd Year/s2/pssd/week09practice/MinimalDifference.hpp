#include <string>
class MinimalDifference
{
	int abs (int a ) // absolute value
	{
		if (a > 0)
		{
			return a;
		}
		return -a;
	}

public:
	int findNumber(int A, int B, int C)
	{
		using namespace std;

		string stringC = to_string(C);
		int sumC = 0, distance = 10000000, result = -1;

		for (int i = 0 ; i < stringC.size(); ++i)
		{
			sumC += int (stringC[i]) - 48;
		}
		
		for (int i = A; i <=B; i++)
		{
			string tempString = to_string (i) ;
			int tempInt = 0 ;
			for (int j = 0 ; j < tempString.size() ; ++j)
			{
				tempInt += int (tempString[j]) - 48;
			}
			if ( distance > abs (tempInt - sumC ) )
			{
				distance = abs (tempInt - sumC );
				result = i;
			}
		}
		return result ;
	}
} ;