class FracCount
{
	int gcd (int a, int b){
		if(b == 0){
			return a;
		}
		return gcd(b,a%b);
	}
public:
	int position(int numerator, int denominator)
	{
		// base case
		if (denominator == 1)
		{
			return 0;
		}

		int count = 0;
		size_t i;
		for (i = 1; i <= numerator; i++)
		{
			if (1 == gcd (i, denominator))
			{
				count++;
			}
		}
		count += position(denominator-2, denominator-1);
		return count;
	}
};