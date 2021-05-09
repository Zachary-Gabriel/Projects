#include <string>
#include <vector>
class PaternityTest
{
public:
	std::vector<int> possibleFathers (std::string child, std::string mother, std::vector<std::string> men)
	{
		std::vector<int> result;
		std::string man;
		size_t lengthDNA = child.size(), lengthDad = men.size(), i, j;
		int sum = 0, arr[lengthDNA];
		bool bad;

		for (i = 0; i < lengthDad; i++)
		{
			man = men[i];
			bad = false;

			for (j = 0; j < lengthDNA; j++)
			{
				arr[j] = 0;
				if (mother[j] == child[j])
				{
					arr[j] = 2;
				}
			}

			for (j = 0; j < lengthDNA; j++)
			{
				if(man[j] == child[j])
				{
					arr[j] = 1;
				}
			}

			sum = 0;
			for (j = 0; j < lengthDNA; j++)
			{
				if (arr[j] == 1) sum += arr[j];
			}
			if (sum < lengthDNA/2) bad = true;
			else
			{
				for (j = 0; j < lengthDNA; j++)
				{
					if (arr[j] == 0)
					{
						bad = true;
						break;
					} 
				}
			}
			if (bad != true) result.push_back(i);
		}
		return result;
	}
};