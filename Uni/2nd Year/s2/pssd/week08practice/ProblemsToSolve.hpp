#include <vector>
#include <iostream>
class ProblemsToSolve
{
public:
	int minNumber(std::vector<int> pleasantness, int variety)
	{
		int smallest = 1000000000,
		largest = 0,
		smallIndex = 0,
		largIndex = 0,
		i,
		length =  pleasantness.size(),
		count = 0;
		bool itExists = false;

		for (i = 0; i < length; ++i)
		{
			if (smallest > pleasantness.at(i))
			{
				smallest = pleasantness.at(i);
				smallIndex = i;
			}
			if (largest < pleasantness.at(i))
			{
				largest = pleasantness.at(i);
				largIndex = i;
			}
			if ( largest - smallest >= variety) 
			{
				itExists = true;
				break;
			}
		}
		if ( !itExists ) return length;
		if (itExists)
		{
			for (int i = 0; i < length; ++i)
			{
				if (smallIndex %2 == 1)
				{
					if (smallIndex <= i) ;
					else
					{
						if (i%2 == 1)
						{
							if (pleasantness[i] - smallest >= variety)
							{
								largIndex = i;
								largest = pleasantness[i];
							}
						}
					}
				}
				if (smallIndex %2 == 0)
				{
					if (smallIndex <= i) ;
					else
					{
						if (i%2 == 0)
						{
							if (pleasantness[i] - smallest >= variety)
							{
								largIndex = i;
								largest = pleasantness[i];
							}
						}
					}
				}
				if (largIndex %2 == 1)
				{
					if (largIndex <= i) ;
					else
					{
						if (i%2 == 1)
						{
							if (largest - pleasantness[i] >= variety)
							{
								smallIndex = i;
								smallest = pleasantness[i];
							}
						}
					}
				}
				if (largIndex %2 == 0)
				{
					if (largIndex <= i) ;
					else
					{
						if (i%2 == 0)
						{
							if (largest - pleasantness[i] >= variety)
							{
								smallIndex = i;
								smallest = pleasantness[i];
							}
						}
					}
				}
			}
		}

		i = 0;
		smallest = 1000000000;
		largest = 0;
		while (i < length)
		{
			++count;
			if (smallest > pleasantness.at(i))
			{
				smallest = pleasantness.at(i);
			}

			if (largest < pleasantness.at(i))
			{
				largest = pleasantness.at(i);
			}

			if ( largest - smallest >= variety) 
			{
				break;
			}

			if (itExists && ( ( i+1 == smallIndex || i+1 == largIndex ) ) )
			{
				++i;
			// std::cout << pleasantness[i] << " single_skip ";
			}
			else
			{
				i += 2;
			// std::cout <<  pleasantness[i] << " double_skip ";
			}
		}
		
		return count;
	}
};