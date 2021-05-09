#include "Node.h"
#include "LinkedList.h"
#include <iostream>
#include <sstream>

int main(void){
	// Setting up input of data
	std::string input;
	std::getline(std::cin,input);
	std::string functionType = "";
	int param1 = 0;
	int param2 = 0;
	int* arr = new int[100];
	int i = 0;
	int j = 0;
	int temp = 0;
	
	// Multiple stages of weeding out the variables requires few boleans
	bool numberContinues = false;
	bool noMoreInts = false;
	bool end_functiontype = false;
	bool end_param1 = false;

	while(input[i]!='\0'){
		// Adding numbers into the array {arr}
		if(noMoreInts == false){
			if(47 < int(input[i]) && int(input[i]) < 58){
				// If the next character in the array is another number
				// then that means its apart of the existing number
				// so the number coninues
				if(47 < int(input[i+1]) && int(input[i+1]) < 58){
					numberContinues = true;
				} else {
					numberContinues = false;
				}
				if(numberContinues == true){
					temp = 10*temp + int(input[i])-48;
				} else {
					// Modifies value so that it is the input number
					// then adds it to the array
					temp = 10*temp + int(input[i])-48;
					arr[j] = temp;
					temp = 0;
					j++; // stores the amount of numbers in the array
				}
			}
			// Checks for a space then another character that isnt a digit, meaning the array is done
			if((i>1) && ((int(input[i])==32)) && (48 > int(input[i+1]) || int(input[i+1]) > 57)){
				noMoreInts = true;
			}
		} 
		if (noMoreInts == true) {
			// step 2) Weeding out the string to a variable functionType 
			if(int(input[i])!=32 && end_functiontype == false){
				functionType += input[i];
				if(int(input[i+1])==32){
					end_functiontype = true;
				}
			} else if(int(input[i])!=32 && end_param1 == false){
				// Then param1
				if(47 < input[i] && input[i] < 58){
					param1 = 10*param1 + (int(input[i])-48);
				}
				if(int(input[i+1])==32){
					end_param1 = true;
				}
			} else {
				// And finally param 2
				if(47 < input[i] && input[i] < 58){
					param2 = 10*param2 + int(input[i])-48;
				}
			}
		}
		i++;
	}

	// Using the array and the variable "j", creating a list then applying the function
	// to it depending on what the input functionType was
	LinkedList *list = new LinkedList(arr, j);
	if(functionType == "AF"){
		list->addFront(param1);
	} else if (functionType == "AE"){
		list->addEnd(param1);
	} else if (functionType == "AP"){
		list->addAtPosition(param1,param2);
	} else if(functionType == "S"){
		list->search(param1);
	} else if(functionType == "DF"){
		list->deleteFront();
	} else if(functionType == "DE"){
		list->deleteEnd();
	} else if(functionType == "DP"){
		list->deletePosition(param1);
	} else if(functionType == "GI"){
		list->getItem(param1);
	}
	// printing the result
	list->printItems();
	delete list;
	return 0 ;
}