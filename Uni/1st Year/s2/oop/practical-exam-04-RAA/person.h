#ifndef PERSON_H
#define PERSON_H
#include <string>

class Person {

	int personID;
	static int nextID;

protected:
	std::string name;
	std::string occupation;
	int service_length;
	int salary;

public:
	Person();
	Person(std::string n, std::string o, int sl);

	int get_personID();
	std::string get_name();
	int get_salary();
	std::string get_occupation();

	void set_name(std::string);
	void set_occupation(std::string);
	virtual void set_salary(int) = 0;
};

#endif