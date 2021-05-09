#ifndef SORT_H
#define SORT_H
#include <vector>
class Sort {
public:
	virtual void sortList(std::vector<int>) = 0;
	void swap(std::vector<int>, int, int);
private:
	std::vector<int> list;
};
#endif