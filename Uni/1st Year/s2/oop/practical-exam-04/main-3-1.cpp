#include "team.h"
#include "person.h"
#include "coach.h"
#include "player.h"
#include "sort_int.h"
#include "sort_string.h"
#include "sort_person.h"

#include <iostream>

int main(){

	team jim = team();
	person** group = jim.get_team();
	for (int i = 0; i < 4; i++){
		person * p = group[i];
		std::cout << p->get_name() << " " << p->get_occupation() << std::endl; 
	}

	// sort int
	int numbers[4] = {3,4,1,2};
	std::cout << std::endl;
	for(int i = 0; i < 4; i++){
		std::cout << numbers[i] << std::endl;
	}

	sort_int::sort(numbers,4);
	std::cout << std::endl;

	for(int i = 0; i < 4; i++){
		std::cout << numbers[i] << std::endl;
	}

	// sort string
	std::string array[4] = {"chad","night","now","algebra"};
	std::cout << std::endl;
	for(int i = 0; i < 4; i++){
		std::cout << array[i] << std::endl;
	}

	sort_string::sort(array,4);
	std::cout << std::endl;

	for(int i = 0; i < 4; i++){
		std::cout << array[i] << std::endl;
	}

	// sort person
	coach * a = new coach("abc",10);
	coach * b = new coach("abb",10);
	coach * c = new coach("name",10);
	coach * d = new coach("aok",10);
	person** people;
	people = new person*[4];

	people[0] = a;
	people[1] = b;
	people[2] = c;
	people[3] = d;
	
	std::cout << std::endl;
	for(int i = 0; i < 4; i++){
		std::cout << people[i]->get_name() << std::endl;
	}

	sort_person::sort(people,4);
	std::cout << std::endl;

	for(int i = 0; i < 4; i++){
		std::cout << people[i]->get_name() << std::endl;
	}

	return 0;
}