#include "Individual.h"
Individual::Individual(int l){
	binaryString = "";
	// Makes string of zeros based on length of string "l"
	for(int i = 0; i < l; i++){
		binaryString += "0";
	}
}
Individual::Individual(std::string n){
	binaryString = "";
	int length = n.length();
	// Makes another string with identical values.
	for(int i =0; i < length; i++){
		binaryString += n[i];
	}
}
int Individual::getLength(){
	return binaryString.length();
}

std::string Individual::getString(){
	return binaryString;
}

int Individual::getBit(int pos){
	// Checks to see if pos is inside binary string
	if(binaryString.length()<=pos){
		return -1;
	}
	return binaryString[pos];
}

void Individual::flipBit(int pos){
	std::string output = "";
	for (int i = 0; i < getLength(); i++){
		// if pos and 0 then one, 
		if (binaryString[i] == '0' && pos==i){
			output += '1';
		//if pos and one then zero
		} else if (binaryString[i] == '1' && pos==i){
			output += '0';
		} else {
			// if not pos
			output+=binaryString[i];
		}
	}
	binaryString = output;
}

int Individual::getMaxOnes(){
	// Storing variables in memory
	int max = 0;
	int temp = 0;
	int length = binaryString.length();
	
	// iterating over string to find how many ones in a row there are
	for(int i =0; i < length; i++){
		if(binaryString[i] == '1'){
			temp++;
			if(temp>max){
				//storing to max {output} if the biggest
				max = temp;
			}
		} else {
			// starts the loop again for the sake of a different substring
			temp = 0;
		}
	}
	return max;
}

