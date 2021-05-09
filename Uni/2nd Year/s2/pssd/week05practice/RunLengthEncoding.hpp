#include <string>
class RunLengthEncoding
{
public:
	std::string decode(std::string text)
	{
		std::string result="";
		size_t i, j, length = text.size(), size = 0;
		int intOf, temp = 0;

		for (i = 0; i < length; i++)
		{
			intOf = int(text[i]);
			if(48 <= intOf && intOf <= 57) // if number
			{
				temp = 10*temp + intOf - 48;
				if (temp >50000) return "TOO LONG";
			}
			else
			{
				if (temp == 0)
				{
					result += text[i];
					size++;
					if (size > 50) return "TOO LONG";
				}
				else 
				{
					for (j = 0; j < temp; j++)
					{
						result += text[i];
						size++;
						if (size > 50) return "TOO LONG";
					}
					temp = 0;
				}
			}
		}
		return result;
	}
};