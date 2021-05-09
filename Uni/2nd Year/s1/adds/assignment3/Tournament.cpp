#include "computer.h"
#include "Tournament.h"
#include "referee.h"
#include <iostream>

#include "RandomComputer.h"
#include "Avalanche.h"
#include "Bureaucrat.h"
#include "Toolbox.h"
#include "Crescendo.h"
#include "PaperDoll.h"
#include "FistfullODollars.h"

std::string Tournament::Part1(std::string player1, std::string player2){
	//intializing everything
	Referee PutMeInTheGameCoach;
	std::string temp;
	Computer* input1;
	Computer* input2;
	input1 = new RandomComputer();
	input2 = new RandomComputer();
	int win = 0;
	int loss = 0;
	int i = 0;
	bool p1 = false;
	bool p2 = false;

	// putting base words in an array for easy iteration
	std::string* base = new std::string[8];
	base[0] = "RandomComputer ";
	base[1] = "Avalanche ";
	base[2] = "Bureaucrat ";
	base[3] = "Toolbox ";
	base[4] = "Crescendo ";
	base[5] = "PaperDoll ";
	base[6] = "FistfullODollars ";
	base[7] = " ";

	// checking what the two contestants are and making their objects
	for(i = 0; i < 8; i++){
		if(player1 == base[i]){
			if(i == 0){
				input1 = new RandomComputer();
			}
			if(i == 1){
				input1 = new Avalanche();
			}
			if(i == 2){
				input1 = new Bureaucrat();
			}
			if(i == 3){
				input1 = new Toolbox();
			}
			if(i == 4){
				input1 = new Crescendo();
			}
			if(i == 5){
				input1 = new PaperDoll();
			}
			if(i == 6){
				input1 = new FistfullODollars();
			}
			if(i == 7){
				p1 = false;
			}
		}
		if(player2 == base[i]){
			if(i == 0){
				input2 = new RandomComputer();
			}
			if(i == 1){
				input2 = new Avalanche();
			}
			if(i == 2){
				input2 = new Bureaucrat();
			}
			if(i == 3){
				input2 = new Toolbox();
			}
			if(i == 4){
				input2 = new Crescendo();
			}
			if(i == 5){
				input2 = new PaperDoll();
			}
			if(i == 6){
				input2 = new FistfullODollars();
			} 
			if(i == 7){
				p2 = true;
			}
		}
	}
	// using p1 & p2 to check if the objects exists or not then putting it through the ref function
	if(p2 == false && p1 == false){
		std::string rounds1 = input1->getRounds();
		std::string rounds2 = input2->getRounds();
		/*
		std::cout << input1->getName() << ": " << rounds1 << std::endl;
		std::cout << input2->getName() << ": " << rounds2 << std::endl;
		*/
		for(i = 0; i < 5; i++){
			temp = PutMeInTheGameCoach.ref(rounds1,rounds2,5);
			if(temp[i]=='W'){
				win++;
			} if(temp[i]=='L'){
				loss++;
			}
		}
		//decides who wins
		if(loss>win){
			return player2;
		} else{
			return player1;
		}
	}
	/* Returning the object instead of the non-object.
	If both are non-objects, the next round will fluch it out.
	If all are non-objects, then the winner is "no input" */

	if(p2 == false){
		return player2;
	}
	else{
		return player1;
	}
}

std::string Tournament::Part2(std::string player1, std::string player2, std::string player3, std::string player4, std::string player5, std::string player6, std::string player7, std::string player8){
	//runs the function above an appropriate amount of times
	Tournament PutMeInTheGameCoach = Tournament();
	std::string winner1 = PutMeInTheGameCoach.Part1(player1,player2);
	std::string winner2 = PutMeInTheGameCoach.Part1(player3,player4);
	std::string winner3 = PutMeInTheGameCoach.Part1(player5,player6);
	std::string winner4 = PutMeInTheGameCoach.Part1(player7,player8);

	std::string winner5 = PutMeInTheGameCoach.Part1(winner1,winner2);
	std::string winner6 = PutMeInTheGameCoach.Part1(winner3,winner4);

	std::string winner = PutMeInTheGameCoach.Part1(winner5,winner6);
	return winner;
}