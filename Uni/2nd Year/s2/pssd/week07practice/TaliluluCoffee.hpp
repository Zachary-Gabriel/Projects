#include <vector>
#include <algorithm>
class TaliluluCoffee
{
public:
	int maxTip(std::vector<int> tips)
	{
		// preallocating memory
		int max = 0, length = tips.size(), time = length-1;

		// sorting the array
		std::sort(tips.begin(), tips.end());

		// going through the list of words from smallest to largest
		for (int i = 0; i <  length; ++i)
		{
			int temp = tips.at(i) - time; // grab the element and modify based on the index (where time = length -i)

			if (temp < 0) temp = 0; // can't have a negative tip

			max += temp; // append that to the output
			--time;
		}
		return max;
	}
};