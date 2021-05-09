#include <iostream>
#include <string>
#include "Tournament.h"
#include "referee.h"
#include "computer.h"
#include "RandomComputer.h"
#include "Avalanche.h"
#include "Bureaucrat.h"
#include "Toolbox.h"
#include "Crescendo.h"
#include "PaperDoll.h"
#include "FistfullODollars.h"

int main(){
	// initializing variables in memory
	std::string str;
	int i = 0;
	int j = 0;
	std::getline(std::cin,str);
	str= str+" ";
	Tournament PutMeInTheGameCoach = Tournament();
	std::string* base = new std::string[8];
	std::string* tourn = new std::string[8];

	for(i=0;i<8;i++){
		tourn[i]=="";
	}
	// putting base words into an array for easy iterations
	base[0] = "RandomComputer ";
	base[1] = "Avalanche ";
	base[2] = "Bureaucrat ";
	base[3] = "Toolbox ";
	base[4] = "Crescendo ";
	base[5] = "PaperDoll ";
	base[6] = "FistfullODollars ";
	base[7] = " ";

	// taking each word and putting it into the tournament function
	for(i = 0; i< 8; i++){
		j=0;
		while(tourn[i]==""){
			if(str.find(base[j]) == 0){
				tourn[i] = base[j];
				if(j == 0){
					str.erase(str.begin(),str.begin()+15);
				}
				if((j == 1) || (j == 4)|| (j == 5)){
					str.erase(str.begin(),str.begin()+10);
				}
				if(j == 2){
					str.erase(str.begin(),str.begin()+11);
				}
				if(j == 3){
					str.erase(str.begin(),str.begin()+8);
				}
				if(j == 6){
					str.erase(str.begin(),str.begin()+17);
				}
			}
			if(j==7){
				tourn[i] = base[j];
				str.erase(str.begin(),str.begin()+str.find(" ")+1);
			}
			j++;
		}
	}
	//running the tournament with appropriate inputs
	std::string result = PutMeInTheGameCoach.Part2(tourn[0],tourn[1],tourn[2],tourn[3],tourn[4],tourn[5],tourn[6],tourn[7]);
	std::cout << result << std::endl;
	return 0;
}