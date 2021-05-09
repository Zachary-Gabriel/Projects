#ifndef meerkat_h
#define meerkat_h

#include <string>

class meerkat 
{
	int age;
	std::string name;
	
public:

	meerkat(); // no name or age is required to create a meerkat
	meerkat(std::string Name, int Age);

	// getters
	std::string getName();
	int getAge();

	// setters
	void setName(std::string meerName);   // change the meerkat's name
	void setAge(int meerAge); // change the meerkat's age

	~meerkat(); // kill the meerkat anikan
};

#endif