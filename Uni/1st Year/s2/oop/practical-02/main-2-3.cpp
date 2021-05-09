#include <cmath>
#include <stdlib.h>
#include<iostream>

extern bool is_a_palindrome(int integers[], int length);
extern int sum_if_a_palindrome(int integers[], int length);
extern int sum_elements(int integers[], int length);

using namespace std;

int main(void){
	int array[4] = {1,2,2,1};
	cout << "is a palidrome 1,2,2,1 " << sum_if_a_palindrome(array,4) << endl;

	int array1[6] = {2,1,0,0,1,2};
	cout << "is a palidrome 2,1,0,0,1,2 " << sum_if_a_palindrome(array1,6) << endl;

	int array2[5] = {1,2,3,2,1};
	cout << "is a palidrome 1,2,3,2,1 " << sum_if_a_palindrome(array2,4) << endl;

	int array3[0] = {};
	cout << "is a palidrome " << sum_if_a_palindrome(array3,0) << endl;

	int array4[4] = {1,3,3,1};
	cout << "is a palidrome 1,3,3,1 " << sum_if_a_palindrome(array4,4) << endl;
	return 0;
}