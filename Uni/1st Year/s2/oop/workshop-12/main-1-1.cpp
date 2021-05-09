#include "animal.h"
#include "vegie.h"
#include "hunter.h"
#include "sort_by_name.h"

#include <string>
#include <iostream>

int main(){
	vegie* aop = new vegie("aop",10);
	vegie* xd = new vegie("xd",10);
	vegie* poop = new vegie("poop",10);

	animal** array = new animal*[3];
	array[0] = poop;
	array[1] = xd;
	array[2] = aop;

	for(int i = 0; i < 3; i++){
		std::string name = array[i]->get_name();
		std::cout<< name << " with ascii: " << int(name[6]) <<std::endl;
	}

	sort_by_name::sort(array,3);
	std::cout <<std::endl;

	for(int i = 0; i < 3; i++){
		std::string name = array[i]->get_name();
		std::cout<< name << " with ascii: " << int(name[6]) <<std::endl;
	}
	return 0;
}