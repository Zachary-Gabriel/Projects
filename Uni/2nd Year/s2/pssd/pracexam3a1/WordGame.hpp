#include <string>
#include <iostream>

class WordGame
{
public:
	int bestScore (std::string s)
	{
		using namespace std;
		int cumsum = 0, i, tempInt;
		int max_index = 0, next_best = 0;
		int letters[26];
		for (i = 0; i < 26; ++i)
		{
			letters[i] = 0;
		}
		for (i = 0 ; i < s.size (); ++i)
		{
			tempInt = int (s[i]) - 65;
			++letters[tempInt] ;
		}
		for (i = 0; i < 26; ++i)
		{
			if (max_index == next_best)
			{
				next_best = i;
				if (letters [max_index] < letters[next_best])
				{
					tempInt = max_index;
					max_index = next_best;
					next_best = tempInt;
				}
			}
			if (letters[max_index] <= letters[i])
			{
				next_best = max_index;
				max_index = i;
			}
			else if (letters[next_best] <= letters[i])
			{
				next_best = i;
			}
		}
		
		for (i = 0; i < 26; ++i)
		{
			if (letters[i] != 0 && i!= max_index && i != next_best ) cout << char (i+65)<< letters[i] << endl;
			tempInt = letters[i];
			if (i == max_index)
			{
				cout << char (i+65) << letters[i] + letters[next_best] << "max_index" << letters[max_index]<< endl;
				tempInt += letters[next_best] ;
			}
			else if (i == next_best)
			{
				cout << char (i+65)<< '0' << letters[i]<< endl;
				continue;
			}
			cumsum += tempInt*tempInt;
		}
		return cumsum;
	}
};