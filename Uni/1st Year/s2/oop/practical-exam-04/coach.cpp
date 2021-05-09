#include "coach.h"
#include "person.h"
#include <string>
	/*
	std::string name;
	std::string occupation;
protected:
	int salary;
	int serviceLength;
	int personID;
	*/
int coach::nextID = 0;
coach::coach(){
	set_name("no name");
	set_occupation("coach");
	serviceLength = 0;
	personID = nextID;
	nextID++;
	salary = 0;
}

coach::coach(std::string n, int sl){
	set_name(n);
	set_occupation("coach");
	serviceLength = sl;
	personID = nextID;
	nextID++;
	salary = 0;
}

int coach::get_salary(){
	if (serviceLength < 10) {
		return salary;
	} else {
		return 3*salary;
	}
}