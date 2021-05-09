#include "sort_person.h"
#include "person.h"
#include "player.h"

void sort_person::sort(person** array, int n){
	std::string a;
	std::string b;
	for (int i = 0; i < n; i ++){
		for (int j = i; j < n; j++){
			a = array[i]->get_name();
			b = array[j]->get_name();
			int k = 0;
			
			while (a[k] == b[k]){
				k++;
			}
			
			if(a[k] > b[k]){
				person * temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}