#include <vector>
#include <string>
class CountExpressions
{
	std::vector<int> results;
	void recursion (int x, int y, int a, int b, int runningTotal)
	{
		if (a == 0 && b == 0)
		{
			results.push_back(runningTotal);
		}

		else if (a == 2 && b == 2)
		{
			results.clear();
			recursion (x, y, a-1, b, x);
			recursion (x, y, a, b-1, y);		
		}
		else
		{
			if(a > 0) 
			{
				recursion (x, y, a-1, b, runningTotal + x);
				recursion (x, y, a-1, b, runningTotal - x);
				recursion (x, y, a-1, b, runningTotal * x);
			}
			if(b > 0) 
			{
				recursion (x, y, a, b-1, runningTotal + y);
				recursion (x, y, a, b-1, runningTotal - y);
				recursion (x, y, a, b-1, runningTotal * y);
			}
		}
	}
public:
	int calcExpressions(int x, int y, int val)
	{
		int tally = 0;
		recursion(x, y, 2, 2, 0);
		for (int i = 0; i < results.size(); i++)
		{
			if(results.at(i) == val)
			{
				tally++;
			}
		}
		return tally;
	}
};
