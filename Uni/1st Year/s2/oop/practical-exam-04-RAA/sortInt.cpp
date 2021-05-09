#include "sortInt.h"

void sort(int* number, int n){
	int temp;
	for(int i = 0; i < n; i ++){
		for(int j = i; j< n; j++){
			if(number[i]>number[j]){
				temp = number[i];
				number[i] = number[j];
				number[j]=temp;
			}
		}
	}
}