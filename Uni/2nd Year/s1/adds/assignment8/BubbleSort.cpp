#include "BubbleSort.h"

int sortList(std::vector<int> list){
	//Storing variables in memory
	int i,j;
	// base cases for a list of less than three values
	if(list.size() == 3)
	{
		if(list.at(0) > list.at(1))
		{
			swap(list,0,1);
			return 0;
		}

		if(list.at(1) > list.at(2))
		{
			swap(list,1,2);
			return 0;
		}
	} else if(list.size() == 2)
	{
		if(list.at(0) > list.at(1))
		{
			swap(list,0,1);
			return 0;
		}
	}
}

void driver(std::vector<int> list){
	int i,j;
	int length = list.size(); // Running function once for optimal time.
	for(i = 0; i < length; i++){
		for(j = 0; j < i; j++){
			if(list.at(i) < list.at(j)){
				swap(list,i,j);
			}
		}
	}
}