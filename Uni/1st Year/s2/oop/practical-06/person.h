#ifndef person_h
#define person_h

#include <string>

class person
{
	std::string name;
	int salary;

public:
	person();
	person(std::string myName,int Salary); // a name and salary must be provided to create a person
	void setName(std::string myName);      // change the person's name
	std::string getName();
	void setSalary(int mySalary);     // change the person's name
	int getSalary();
	~person(); // destroy all humans 2
};

#endif