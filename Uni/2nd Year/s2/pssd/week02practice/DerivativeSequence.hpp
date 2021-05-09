#include <vector>
class DerivativeSequence 
{
public:
	std::vector<int> derSeq(std::vector<int> a, int n)
	{
		std::vector<int> b;
		b.clear();
		for(int i = 0; i < n; i++){
			for(int j = 0; j < a.size()-1; j++){
				b.push_back(a.at(j+1) - a.at(j)); 
			}
			a.clear();
			a = b;
			b.clear();
		}
		return a;
	}
};