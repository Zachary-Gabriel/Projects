#include<iostream>
#include <cmath>
using namespace std;

/*
 Please note, you must use recursion in answering this question.
 */

/*
 Implement the following function based on the given description.
 */
int log5(int i)
{
	if(i==1){
		return 0;
	}
    if(i<6){
    	return 1;
    }
    return 1+ log5(round(i/5));
}

// You are not supposed to change the main function
int main() {
    cout << log5(1)<< endl; //
    cout << log5(5)<< endl; //
    cout << log5(26)<< endl; //
    cout << log5(1026)<< endl; //
}
