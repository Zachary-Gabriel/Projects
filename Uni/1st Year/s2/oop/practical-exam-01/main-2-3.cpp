#include <iostream>

extern int multiply_if_threes(int integers[], int length);
extern bool is_ascending_multiple_threes(int integers[], int length);
extern int multiply_elements(int integers[],int length);

using namespace std;

int main(void){
	int array[3] = {3,6,9};
	int LENGTH = 3;

	cout << array << " " << multiply_if_threes(array,LENGTH) << endl;
	return 0;
}