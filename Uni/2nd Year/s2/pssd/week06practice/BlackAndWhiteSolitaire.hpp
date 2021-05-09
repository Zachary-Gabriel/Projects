#include <string>
class BlackAndWhiteSolitaire
{
public:
	int minimumTurns(std::string cardFront)
	{
		int i = 0, length = cardFront.size(), tally1 = 0, tally2 = 0;
		for (i = 0; i < length; i++)
		{
			if (i%2 == 0)
			{
				if (cardFront[i] == 'B') tally1++;
				else tally2++;
			}
			else
			{
				if (cardFront[i] == 'W') tally1++;
				else tally2++;
			}
		}
		if (tally2 > tally1)
		{
			return tally1;
		}
		return tally2;
	}
};