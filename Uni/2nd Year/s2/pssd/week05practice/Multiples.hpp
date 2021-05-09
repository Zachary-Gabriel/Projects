class Multiples
{
public:
	int number(int min, int max, int factor)
	{
		int i, result = 0;
		for (i = min; i <= max; i++)
		{
			if (i%factor == 0)
			{
				result++;
			}
		}
		return result;
	}
};