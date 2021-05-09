#include <vector>

class ColorfulRabbits
{
public:
	// noot
	int getMinimum (std::vector<int> replies)
	{
		int current = replies.at (0) ;
		int result = current + 1 ;
		for (int i = 0 ; i < replies.size () ; ++i)
		{
			if (replies.at (i) != current || replies.at (i) == 0)
			{
				current = replies.at (i) ;
				result += current + 1 ;
			}
		}
		return result ;
	}
};