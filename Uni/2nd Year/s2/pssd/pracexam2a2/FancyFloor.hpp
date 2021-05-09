#include <vector>
#include <string>

class FancyFloor
{
public:
	int howmanyBoards (std::vector<std::string> layout)
	{
		int length = layout.size();
		std::string lastLine, line = layout.at(0);
		int lineLength = line.size();
		int boards = 0;

		for (int i = 0; i < length; i++)
		{
			line = layout.at(i); // line
			for (int j = 0; j < lineLength; j++)
			{
				boards++;
				if (j > 0 && line[j-1] == '-' && line[j] == '-') boards--;
				else if (i > 0 && line[j] == '|' && lastLine[j] == '|') boards--;
			}
			lastLine = line;
		}
		return boards;
	}
};