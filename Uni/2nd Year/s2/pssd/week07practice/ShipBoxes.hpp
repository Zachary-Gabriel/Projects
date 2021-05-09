#include <vector>
class ShipBoxes
{
	int max (int a, int b)
	{
		if (a > b) return a;
		return b;
	}
public:
	int bestCost(std::vector<int> box1, std::vector<int> box2)
	{
		int depth, width, height, min = 2147483647, temp;
		for(int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				depth = box1.at(i) + box2.at(j);
				for (int k = 0; k < 3; k++)
				{
					if (i==k) continue;
					for (int l = 0; l < 3; l++)
					{
						if (l==j) continue;
						int i2,j2;
						if (i == 0 && k == 1) i2 = 2;
						if (i == 0 && k == 2) i2 = 1;
						if (i == 1 && k == 0) i2 = 2;
						if (i == 1 && k == 2) i2 = 0;
						if (i == 2 && k == 0) i2 = 1;
						if (i == 2 && k == 1) i2 = 0;
						
						if (j == 0 && l == 1) j2 = 2;
						if (j == 0 && l == 2) j2 = 1;
						if (j == 1 && l == 0) j2 = 2;
						if (j == 1 && l == 2) j2 = 0;
						if (j == 2 && l == 0) j2 = 1;
						if (j == 2 && l == 1) j2 = 0;
						width = max (box1.at(k), box2.at(l));
						height = max (box1.at(i2), box2.at(j2));

						temp = 2*(height * depth) + 2*(width * depth) + 4 * (width * height);
						if (min > temp) min = temp;
						temp = 4*(height * depth) + 2*(width * depth) + 2 * (width * height);
						if (min > temp) min = temp;
						temp = 2*(height * depth) + 4*(width * depth) + 4 * (width * height);
						if (min > temp) min = temp;
					}
				}
			}
		}
		return min;
	}
};