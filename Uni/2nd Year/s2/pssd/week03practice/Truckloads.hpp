#include <cmath>
class Truckloads
{
public:
	int numTrucks(int numCrates, int loadSize)
	{
		if (loadSize >= numCrates)
		{
			return 1;
		}
		if (numCrates%2 == 1)
		{
			return numTrucks(floor(numCrates/float(2)), loadSize) + numTrucks(ceil(numCrates/float(2)), loadSize);
		}
		else
		{
			return 2 * numTrucks(numCrates/2, loadSize);
		}
	}
};
