#include "JumpyNum.hpp"
#include "GoodHours.hpp"
#include "ColorfulRabbits.hpp"
#include "BridgeSort.hpp"
#include "BigBurger.hpp"

#include <iostream>
#include <vector>

using namespace std;

int main (void)
{
	JumpyNum j ;
	cout << j.howMany (1, 10) << + " should be 9" << endl ; 
	cout << j.howMany (9, 23) << + " should be 9" << endl ; 
	cout << j.howMany (2000000000, 2000000000) << + " should be 0" << endl ; 
	cout << j.howMany (8000, 20934) << + " should be 3766" << endl ; 
	cout << j.howMany (1, 100000000) << + " should be 3766" << endl ; 
	
	GoodHours g;
	cout << g.howMany ("11:11", "11:11") << " should be 1" << endl;
	cout << g.howMany ("00:56", "12:32") << " should be 164" << endl;
	cout << g.howMany ("00:00", "00:59") << " should be 60" << endl;
	cout << g.howMany ("10:10", "10:20") << " should be 2" << endl;
	cout << g.howMany ("23:10", "01:23") << " should be 75" << endl;

	ColorfulRabbits c;
	cout << c.getMinimum ({ 1, 1, 2, 2 }) << " should be 5" << endl ;
	cout << c.getMinimum ({ 0 }) << " should be 1" << endl ;
	cout << c.getMinimum ({ 2, 2, 44, 2, 2, 2, 444, 2, 2 }) << " should be 499" << endl ;

	BridgeSort b;
	cout << b.sortedHand ("HAH2H3C4D5ST") << endl << "C4D5H2H3HAST" << endl ;
	cout << b.sortedHand ("H3SAHA") << endl << "H3HASA" << endl ;
	cout << b.sortedHand ("HACADASA") << endl ;

	BigBurger bb;
	cout << bb.maxWait ({3,3,9}, {2,15,14}) << " should be 11" << endl ;
	cout << bb.maxWait ({182}, {11}) << " should be 0" << endl ;
	cout << bb.maxWait ({2,10,11}, {3,4,3}) << " should be 3" << endl ;
	cout << bb.maxWait ({2,10,12}, {15,1,15}) << " should be 7" << endl ;

	return 0 ;
}