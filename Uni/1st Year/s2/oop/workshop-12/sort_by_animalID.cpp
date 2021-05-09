#include "animal.h"
#include "vegie.h"
#include "hunter.h"
#include "sort_by_animalID.h"

#include <string>

void sort_by_animalID::sort(animal **animals,int n){
	animal* temp;
	for (int i = 0; i < n; i++){
		for (int j = i; j < n; j++){
			if(animals[i]->get_animalID() > animals[j]->get_animalID()){
				temp = animals[j];
				animals[j] = animals[i];
				animals[i] = temp;
			}
		}
	}
}