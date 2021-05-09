#include <string>
#include <vector>
#include <iostream>
class Thimbles
{
public:
	int thimbleWithBall(std::vector<std::string> swaps)
	{
		int pos = 1,
			length = swaps.size();
		unsigned i;

		for (i = 0; i < length; ++i)
		{
			std::string line = swaps.at(i);
			if ( int (line[0]) == pos + 48 )
			{
				pos = int(line[2] - 48);
			}
			else if ( int (line[2]) == pos + 48 )
			{
				pos = int(line[0] - 48);
			}
		}
		return pos;
	}
};