#include <vector>
#include <string>
#include <iostream>
class SortingBoxes
{
public:
	int minOp (std::vector<int> r, std::vector<int> g, std::vector<int> b)
	{
		if (r.size () < 2)
		{
			for (int i = 0; i < r.size() ; ++i)
			{
				if (r[0] == 0 && g[0] == 0)
				{
					return 0;
				}
				if (r[0] == 0 && b[0] == 0)
				{
					return 0;
				}
				if (b[0] == 0 && g[0] == 0)
				{
					return 0;
				}
				else return -1;
			}
		}
		if (r.size () < 3)
		{
			if (r[0] == 0 && r[1] == 0)
			{
				if (g[0] + b[1] < g[1] + b[0])
				{
					return g[0] + b[1];
				}
				else
				{
					g[1] + b[0];
				}
			}
			if (g[0] == 0 && g[1] == 0)
			{
				if (r[0] + b[1] < r[1] + b[0])
				{
					return r[0] + b[1];
				}
				else
				{
					r[1] + b[0];
				}
			}
			if (b[0] == 0 && b[1] == 0)
			{
				if (g[0] + r[1] < g[1] + r[0])
				{
					return g[0] + r[1];
				}
				else
				{
					g[1] + r[0];
				}
			}
		}
		return -1;
	}
	
};