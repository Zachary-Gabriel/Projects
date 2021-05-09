#include <vector>
using namespace std;
class TalentComp
{
public:
	
	int round2(vector<int> talent, vector<int> skill)
	{
		int N = talent.size(), i, j, advance = 0;
		bool fail = false;
		for (i = 0; i < N; i++)
		{
			fail = false;
			for (j = 0; j < N; j++)
			{
				if (i==j) continue;
				if (talent.at(i) < talent.at(j) && skill.at(i) < skill.at(j))
				{
					fail = true;
				}
			}
			if (fail == false) advance++;
		}
		return advance;
	}
};