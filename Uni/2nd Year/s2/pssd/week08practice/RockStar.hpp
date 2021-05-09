class RockStar
{
	int min (int a, int b)
	{
		if (a < b) return a;
		return b;
	}
	int max (int a, int b)
	{
		if (a > b) return a;
		return b;
	}
public:
	int getNumSongs(int ff, int fs, int sf, int ss)
	{
		if (ff == 0)
		{
			if (fs == 0)
			{
				if (sf == 0)
				{
					return ss; // as only number -> good
				}
				else
				{
					return ss+1; // tack onto end -> good
				}
			}
			else
			{
				if (sf == 0)
				{
					return ss + 1; // tack onto start -> good
				}
				else
				{
					if (fs <= sf)
					{
						return 2*fs + ss;
					}
					else
					{
						return 2*sf + 1 + ss;
					}
				}
			}
		}
		else
		{
			if (fs == 0)
			{
				return ff;
			}
			else
			{
				if (sf == 0)
				{
					return ff + 1 + ss;
				}
				else
				{
					if (fs <= sf)
					{
						return ff + 2*fs + ss;
					}
					else
					{
						return ff + 2*sf + 1 + ss;
					}
				}
			}
		}
	}
};