#include <string> // for strings
#include <vector> // for vectors
#include <algorithm> // for sorting
#include <iostream> // for error testing

class PrefixFreeSets
{
public:
	int maxElements(std::vector<std::string> words)
	{
		//preallocating memory
		std::string first, second;
		int i, j, length = words.size(), count = 1;

		std::sort(words.begin(), words.end()); // sort the list in order from smallest to largest
		first = words.at(length-1); // start at the last word in the list

		for (i = length-1; i >= 0; --i) 
		{
			// grab the elements of list of words
			second = words.at(i);

			// finding the larger word
			int var = first.size();
			if (var > second.size() ) var = second.size(); 

			// if the words are not equal
			if (first.compare(0,var,second) != 0) 
			{
				first = second; // update the comparison word
				count++; // then add one to the output
			}
		}
		return count;
	}
};