#include "Queue.h"

Queue::Queue(){
	//initialising variables
	start = 0;
	end = 0;
	count = 0;
}

void Queue::add(int val){
	//stretching end
	end++;
	// making sure we're in the array bounds
	if (count == CAP){
		count = 0;
	}
	// adding value to array if start <= end
	if(start <= end){
		content[count] = val;
		count++;
	}
}

int Queue::remove(){
	// pushing the start back
	start++;
}