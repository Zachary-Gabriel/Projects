#ifndef PERSON_H
#define PERSON_H

#include <string>

class person{
	std::string name;
	std::string occupation;
protected:
	int salary;
	int serviceLength;
	int personID;
public:
	person();
	person(std::string n, std::string o, int sl);

	//setters
	void set_name(std::string);
	void set_occupation(std::string);
	void set_salary(int);

	//getters
	std::string get_name();
	std::string get_occupation();
	virtual int get_salary() = 0;
	int get_personID();
};
#endif