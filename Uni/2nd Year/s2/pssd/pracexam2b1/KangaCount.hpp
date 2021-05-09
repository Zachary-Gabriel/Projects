class KangaCount
{
	double pow (double var, int power)
	{
		if (power == 0)
		{
			return 1;
		}
		return var * pow(var,power-1);
	}
public:
	double howProbable (int estimatedK, int caughtperday, int daysNumber, int markedK)
	{
		double result = 1;
		for (int i = 0; i < daysNumber; i++)
		{
			result = caughtperday * pow(result/double(estimatedK),daysNumber-1);
		}
		return result;
	}
};