#include <string>
class StrangeComputer
{
public:
	int setMemory(std::string mem)
	{
		char comparison = '0';
		int result = 0;
		size_t length = mem.size(), i;
		for (i = 0; i < length; i++)
		{
			if (mem[i] != comparison)
			{
				result++;
				if(comparison == '0')
				{
					comparison = '1';
				}
				else
				{
					comparison = '0';
				}
			}
		}
		return result;
	}
};
