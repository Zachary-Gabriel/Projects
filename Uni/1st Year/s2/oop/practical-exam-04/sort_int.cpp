#include "sort_int.h"

void sort_int::sort (int* numbers, int n){
	int temp;
	for (int i = 0; i < n; i ++){
		for (int j = i; j < n; j++){
			if(numbers[j] < numbers[i]){
				temp = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = temp;
			}
		}
	}
}