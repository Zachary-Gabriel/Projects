#include "Palindrome.h"
#include <iostream>
#include <string>
#include <typeinfo>

std::string Palindrome::lowerCase(std::string str){
	int length = 0;
	int i = 0;
	int temp;

	// finding amount of characters in the string
	while(str[length] != '\0'){
		length++;
	}
	
	// checking if only letters
	for(i = 0; i < length; i++){
		if((int(str[i]) > 64 && int(str[i]) < 91) || (int(str[i]) > 96 && int(str[i]) < 123)){
		} else {
			str = removeNonLetters(str);
		}
	}
	// turining everythign into lowercase
	for(i = 0; i < length; i++){
		if(int(str[i]) > 64 && int(str[i]) < 91){
			temp = int(str[i])+32;
			str[i] = char(temp);
		}
	}
	return str;
}

std::string Palindrome::isPalindrome(std::string str){
	int length = 0;
	int i = 0;
	int j = 0;
	// find length
	while(str[length] != '\0'){
		length++;
	}

	// check to see if previous steps are done
	for(i = 0; i < length; i++){
		if(int(str[i]) > 96 && int(str[i]) < 123){
		} else {
			str = removeNonLetters(str);
		}
	}
	// check if palindrome
	j = length-1;
	for(i = 0; i < length; i++){
		if(str[i] != str[j]){
			return "No";
		}
		j--;
	}
	return "Yes";
}

std::string Palindrome::removeNonLetters(std::string oldString){
	std::string newString = "";
	int length = 0;
	int temp = 0;
	int i = 0;

	// find length
	while(oldString[temp] != '\0'){
		temp++;
	}

	// find where the nonletters are
	int array[temp];
	for(i = 0; i < temp; i++){
		if((int(oldString[i]) > 64 && int(oldString[i]) < 91) || (int(oldString[i]) > 96 && int(oldString[i]) < 123)){
			array[i] = 1;
			length++;
		} else {
			array[i] = 0;
		}
	}

	// making new string where nonletters are removed
	int j = 0;
	for(i = 0; i < length; i++){
		while(array[j] != 1 ){
			j++;
		}
		newString = (newString+oldString[j]);
		j++;
	}
	// run next steps
	newString = lowerCase(newString);
	newString = isPalindrome(newString);
	return newString;
}