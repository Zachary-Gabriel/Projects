#include <iostream>

class ChristmasTree
{
	int fact(int n)
	{
		if (n > 0)
		{
			return n * fact(n-1);
		}
		else
		{
			return 1;
		}
	}
public:
	long decorationWays(int N, int r, int g, int b)
	{
		long combinations = 0;
		long temp;
		if (N <= 0 )
		{
			return 0;
		}
		if (N == 1)
		{
			// no colours
			if (b == 0 && g == 0 && r == 0) 
			{
				return 0;
			}
			// one colour
			if (r >=1 && g == 0 && b == 0) 
			{
				return 1;
			}
			else if (g >=1 && r == 0 && b == 0) 
			{
				return 1;
			}
			else if (b >=1 && g == 0 && r == 0) 
			{
				return 1;
			}
			// two colours
			if (r >0 && g > 0 && b == 0) 
			{
				return 2;
			}
			else if (g > 0 && r == 0 && b > 0) 
			{
				return 2;
			}
			else if (b > 0 && g == 0 && r > 0) 
			{
				return 2;
			}
			// three colours
			else if (b > 0 && g > 0 && r > 0) 
			{
				return 3;
			}
			// no colours (again)
			else 
			{
				return 0;
			}
		}

		// case where all 3 colours are used
		if (N%3 == 0 && r >= N/3 && g >= N/3 && b >= N/3)
		{
			temp = decorationWays(N-1, r-N/3, g-N/3, b-N/3);
			combinations += fact(N)/(fact(N/3)*fact(N/3)*fact(N/3)) * temp;
		}
		// cases where 2 colours are used
		if (N%2 == 0)
		{ 
			if (r >= N/2 && g >= N/2)
			{
				temp = decorationWays(N-1, r-N/2, g-N/2, b);
				combinations += fact(N)/(fact(N/2)*fact(N/2)) * temp;
			}
			if (r >= N/2 && b >= N/2)
			{
				temp = decorationWays(N-1, r-N/2, g, b-N/2);
				combinations += fact(N)/(fact(N/2)*fact(N/2))  * temp;
			}
			if (b >= N/2 && g >= N/2)
			{
				temp = decorationWays(N-1, r, g-N/2, b-N/2);
				combinations += fact(N)/(fact(N/2)*fact(N/2)) * temp;
			}
		}
		// cases with 1 colour1
		if (r >= N)
		{
			temp = decorationWays(N-1, r-N, g, b);
			combinations += temp;			
		}
		if (b >= N)
		{
			temp = decorationWays(N-1, r, g, b-N);
			combinations += temp;			
		}
		if (g >= N)
		{
			temp = decorationWays(N-1, r, g-N, b);
			combinations += temp;			
		}
		return combinations;
	}
};