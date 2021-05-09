#include <iostream>

extern int maximum_sum(int *nums,int length);

int main(void) {
	int array[10] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
	std::cout << maximum_sum(array,10) << std::endl;
	return 0;
}