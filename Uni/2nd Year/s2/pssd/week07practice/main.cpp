#include "PrefixFreeSets.hpp"
#include "LostParentheses.hpp"
#include "TaliluluCoffee.hpp"
#include "ShipBoxes.hpp"

#include <iostream>
#include <vector>
int main ()
{
	PrefixFreeSets p;
	std::vector<std::string> words, words1, words2, words3;
	words = {"hello","hi","h","run","rerun","running"};
	words1 = {"a","b","cba","cbc","cbb","ccc"};
	words2 = {"a","ab","abc","abcd","abcde","abcdef"};
	words3 = {"topcoder","topcoder","topcoding"};
	std::cout << "output: " << p.maxElements(words) << " , expected: 4" << std::endl;
	std::cout << "output: " << p.maxElements(words1) << " , expected: 6" << std::endl;
	std::cout << "output: " << p.maxElements(words2) << " , expected: 1" << std::endl;
	std::cout << "output: " << p.maxElements(words3) << " , expected: 2" << std::endl;

	// LostParentheses l;
	// std::cout << "output: " << l.minResult("55-50+40") << " , expected: -35" << std::endl;
	// std::cout << "output: " << l.minResult("10+20+30+40") << " , expected: 100" << std::endl;
	// std::cout << "output: " << l.minResult("00009-00009") << " , expected: 0" << std::endl;
	
	// TaliluluCoffee t;
	// std::vector<int> tips1 = {3, 3, 3, 3};
	// std::vector<int> tips2 = {3, 2, 3};
	// std::vector<int> tips3 = {7, 8, 6, 9, 10};
	// std::vector<int> tips4 = {1, 1, 1, 1, 2};
	// std::vector<int> tips5  = {1, 2, 3};

	// std::cout << "output: " << t.maxTip(tips1)<< " , expected: 6" << std::endl;
	// std::cout << "output: " << t.maxTip(tips2)<< " , expected: 5" << std::endl;
	// std::cout << "output: " << t.maxTip(tips3)<< " , expected: 30" << std::endl;
	// std::cout << "output: " << t.maxTip(tips4)<< " , expected: 2" << std::endl;
	// std::cout << "output: " << t.maxTip(tips5)<< " , expected: 4" << std::endl;
	
	// ShipBoxes s;
	// std::vector<int> words11, words12, words21, words22, words31, words32, words41, words42;
	// words11 = {1,4,9};
	// words12 = {1,4,9};
	// words21 = {1,1,1};
	// words22 = {1,1,1};
	// words31 = {1,9000,9000};
	// words32 = {40,40,40};
	// words41 = {6570,6076,5880};
	// words42 = {7595,3,1835};
	// std::cout << "output: " << s.bestCost(words11,words12) << " , expected: 140" << std::endl;
	// std::cout << "output: " << s.bestCost(words21,words22) << " , expected: 12" << std::endl;
	// std::cout << "output: " << s.bestCost(words31,words32) << " , expected: 164214000" << std::endl;
	// std::cout << "output: " << s.bestCost(words41,words42) << " , expected: 324635290" << std::endl;

	return 0;
}