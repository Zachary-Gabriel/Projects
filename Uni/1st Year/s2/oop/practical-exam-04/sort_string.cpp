#include "sort_string.h"

void sort_string::sort (std::string* array, int n){
	std::string temp;
	std::string a;
	std::string b;
	for (int i = 0; i < n; i ++){
		for (int j = i; j < n; j++){
			a = array[i];
			b = array[j];
			int k = 0;
			
			while (a[k] == b[k]){
				k++;
			}
			
			if(a[k] > b[k]){
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}