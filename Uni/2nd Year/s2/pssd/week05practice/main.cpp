#include <iostream>
#include "RGBStreet.hpp"
// #include "SimpleDuplicateRemover.hpp"
// #include "Multiples.hpp"
#include "RunLengthEncoding.hpp"
#include <vector>

int main ()
{
	// RGBStreet p;
	// std::vector<std::string> poop;
	// poop = {"30 19 5", "64 77 64", "15 19 97", "4 71 57", "90 86 84", "93 32 91"};
	// std::cout << p.estimateCost(poop) << std::endl;

	// SimpleDuplicateRemover u;
	// std::vector<int> poop;
	// poop = {1,5,5,1,6,1};
	// std::vector<int> q;
	// q = u.process(poop);
	// for (int i = 0; i < q.size(); i++)
	// {
	// 	std::cout << q.at(i) << ' ';
	// }
	// std::cout <<'\n';

	// Multiples m;
	// std::cout << m.number(-75312, 407891, 14) << std::endl;

	RunLengthEncoding h;
	std::cout << h.decode("9223372036854775821F") << std::endl;
	std::cout << "ABCDE" << std::endl;
	return 0;
}