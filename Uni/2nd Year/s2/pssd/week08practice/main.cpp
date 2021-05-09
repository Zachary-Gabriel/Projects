#include "HandsShaking.hpp"
#include "ProblemsToSolve.hpp"
#include "Thimbles.hpp"
#include "RockStar.hpp"

#include <iostream>
#include <vector>

using namespace std ;

int main ()
{
	 HandsShaking h ;
	 cout << h.countPerfect(8) << '\n' ;
	 cout << h.countPerfect(10) << '\n' ;
	 cout << h.countPerfect(6) << '\n' ;
	 cout << h.countPerfect(4) << '\n' ;

	// ProblemsToSolve p;
	// std::vector<int> v1 = {1, 2, 3}, v2 = {1, 2, 3, 4, 5}, v3 = {10, 1, 12, 101}, 
	// 				 v4 = {6, 2, 6, 2, 6, 3, 3, 3, 7}, v5 ={1,2,3,4,5},
	// 				 v6 = {1,1}, v7 = {2,2,2}, 
	// 				 v8 = {9, 11, 2, 7, 3, 1, 15, 2, 15, 14, 15, 4, 13, 7, 3, 12, 17, 7, 4, 3};
	// cout << p.minNumber(v1, 2) << '\n' ;
	// cout << p.minNumber(v2, 4) << '\n' ;
	// cout << p.minNumber(v3, 100) << '\n' ;
	// cout << p.minNumber(v4, 4) << '\n' ;
	// cout << p.minNumber(v5, 100000) << '\n' ;
	// cout << p.minNumber(v6, 1) << '\n' ;
	// cout << p.minNumber(v7, 1) << '\n' ;
	// cout << p.minNumber(v8, 11) << '\n' ;

	// Thimbles y;
	// cout << y.thimbleWithBall({"1-2", "3-2", "1-2", "2-1", "2-1", "3-2", "1-3", "3-1", "1-2"}) << '\n' ; 
	// cout << y.thimbleWithBall({"2-3", "1-3", "2-3", "2-1", "3-1"}) << '\n' ; 
  /*	
RockStar r;
	cout << r.getNumSongs(100, 0, 0, 200) << " wants 100" <<'\n';
	
	cout << r.getNumSongs(0, 0, 20, 200) << " wants 201" <<'\n';
	cout << r.getNumSongs(1,2,1,1) << " wants 5" <<'\n';
	cout << r.getNumSongs( 192, 279, 971, 249) << " wants 999" <<'\n';
	cout << r.getNumSongs( 1000, 1000, 1000, 1000) << " wants 4000" <<'\n';
	*/
	return 0 ;
}
