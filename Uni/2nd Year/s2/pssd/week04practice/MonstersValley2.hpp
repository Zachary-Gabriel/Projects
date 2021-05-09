#include <vector>
#include <iostream>
class MonstersValley2
{
public:
	int minimumPrice (std::vector<int> dread, std::vector<int> price)
	{
		int pPrice = 0, length = dread.size(), result = 434, result1;
		long pDread=0, dred11, dred12, dred2, temp;
		size_t i, j, itemp, i1, i2, i3;
		int array[length];

		for (i = 0; i < length; i++)
		{
			array[i] = 0;
		}

		for (i = 0; i < length; i++)
		{
			if (pDread < dread.at(i))
			{
				if (i > 0)
				{
					dred11 = 0;
					dred12 = 0;
					dred2 = 0;
					i2=0;
					// check if a 2 is > the 2 largest 1's
					for (j = 0; j <= i; j++)
					{
						if (price.at(j) == 1 && array[j] == 0)
						{
							if (dred11 < dread.at(j))
							{
								dred11 = dread.at(j);
								i1 = j;
								if (dred11 > dred12)
								{
									temp = dred12;
									dred12 = dred11;
									dred11 = temp;
									itemp = i2;
									i2 = i1;
									i1 = itemp;
								}
							}
						}
						else
						{
							if (dred2 < dread.at(j) && array[j] == 0)
							{
								dred2 = dread.at(j);
								i3 = j;
							}
						}
					}
					if (dred12+pDread >= dread.at(i) && i2 != 0)
					{
						pDread += dread.at(i2);
						pPrice += price.at(i2);
						array[i2] = 1;
						// std::cout << i2 << ' ';
					}
					else if (dred11+dred12 > dred2)
					{
						pDread += dread.at(i2);
						pPrice += price.at(i2);
						// std::cout << i2 << ' ';
						array[i2] = 1;
						pDread += dread.at(i1);
						pPrice += price.at(i1);
						array[i1] = 1;
						// std::cout << i1 << ' ';
					}
					else 
					{
						pDread += dread.at(i3);
						pPrice += price.at(i3);
						array[i3] = 1;
						// std::cout << i3 << ' ';
					}
				}
				else
				{
					// std::cout << i << ' ';
					array[i] = 1;
					pDread += dread.at(i);
					pPrice += price.at(i);
				}
			}
		}
		// std::cout << std::endl;

		result1 = pPrice;
		for (j = 0; j < length; j++)
		{
			for (i = 0; i < j; i++)
			{
				pDread += dread.at(i);
				pPrice += price.at(i);
				// std::cout << pDread << std::endl;
			}

			for (i = j; i < length; i++)
			{
				if (pDread < dread.at(i))
				{
					pDread += dread.at(i);
					pPrice += price.at(i);
				}
			}
			if (result > pPrice)
			{
				// std::cout << result << std::endl;
				result = pPrice;
			}
			pDread = 0;
			pPrice = 0;
		}
		if (result1 > result) return result;
		return result1;
	}
};