#include <iostream>

using namespace std;

void copy_2d_strings(string first[][2], string second[][2], int rows) {
	for (int i =0; i < rows; i++) {
		for (int j = 0; j < 2; j++) {
			second[i][j] = first[i][j];
		}
	}
}