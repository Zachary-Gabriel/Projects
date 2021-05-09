#ifndef ANIMAL_H
#define ANIMAL_H
#include <string>

class animal
{
private:
	int volume;
protected:
	std::string name;
	int animalID;
public:
	animal();
	animal(std::string, int);
	~animal();

	int get_volume();
	int get_animalID();
	virtual std::string get_name() = 0;

	void set_volume(int);
	void set_name(std::string);
};
#endif