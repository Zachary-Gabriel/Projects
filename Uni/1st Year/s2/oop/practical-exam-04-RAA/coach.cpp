#include <string>
#include "person.h"
#include "coach.h"

Coach::Coach(){
	occupation = "coach";
	name = "";
	service_length=0;
}

Coach::Coach(std::string n, int sl{
	name = n;
	occupation = "coach";
	service_length = sl;
}

void Coach::set_salary(int s){
	salary = 5000*service_length + s;
}