#include "sort_by_name.h"
#include "animal.h"
#include "vegie.h"
#include "hunter.h"


#include <string>

void sort_by_name::sort(animal **animals, int n){
	animal* temp;
	for (int i = 0; i < n; i++){
		for (int j = i; j < n; j++){

			std::string nameA= animals[i]->get_name();
			std::string nameB= animals[j]->get_name();
			int k=0;
			while(nameA[k] == nameB[k]){
				k++;
			}
			 
			if(int(nameA[k]) > int(nameB[k])){
				temp = animals[j];
				animals[j] = animals[i];
				animals[i] = temp;
			}
		}
	}
}