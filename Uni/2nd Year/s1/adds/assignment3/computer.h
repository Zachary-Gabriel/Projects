#ifndef COMPUTER_H
#define COMPUTER_H
#include <string>
class Computer{
protected:
	std::string name;
	std::string rounds;
	int length;
public:
	Computer();
	~Computer();
	std::string getName();
	int getLength(std::string);
	virtual std::string getRounds() = 0;
};
#endif