#include <iostream>

using namespace std;

extern void copy_2d_strings(string first[][2], string second[][2], int rows) ;

int main(void) {

	string array1[3][2] = {{"aa","bb"},
							{"aa","bb"},
							{"aa","bb"}};
	string array2[3][2];
	copy_2d_strings(array1,array2,3);

	return 0;
}