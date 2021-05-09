#include <string>
class LostParentheses
{
public:
	int minResult(std::string e)
	{
		int i, var = 0, length = e.size(), cumSum = 0;
		bool isNeg = false;
		for (i = 0; i < length; ++i) // looping through loop to peel out the numbers
		{
			if (48 <= int(e[i]) && int(e[i]) <= 57) // if another int, append to variable
			{
				var = 10*var + int(e[i])-48;
			}
			
			if (e[i] == '+')
			{
				if (isNeg)
				{
					var = -1 * var;
					std::cout << var << " ";
					cumSum += var;
				}
				else 
				{
					std::cout << var << " ";
					cumSum += var;
				}
				var = 0;
			}

			if (e[i] == '-')
			{
				if (isNeg)
				{
					var = -1 * var;
					std::cout << var << " ";
					cumSum += var;
				}
				else 
				{
					std::cout << var << " ";
					cumSum += var;
				}
				isNeg = true;
				var = 0;
			}
		}
		//dealing with last number
		if (isNeg)
		{
			var = -1 * var;
			std::cout << var << ": ";
			cumSum += var;
		}
		else 
		{
			std::cout << var << ": ";
			cumSum += var;
		}
		return cumSum;
	}
};