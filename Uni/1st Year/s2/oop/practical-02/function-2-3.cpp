extern bool is_a_palindrome(int integers[], int length);
extern int sum_if_a_palindrome(int integers[], int length);
extern int sum_elements(int integers[], int length);

int sum_if_a_palindrome(int integers[], int length){
	if (length < 1){
		return -1;
	} else {
		if(is_a_palindrome(integers, length) == true){
			int Return = sum_elements(integers, length);
			return Return;
		} else {
			return -2;
		}
	}
}

bool is_a_palindrome(int integers[], int length){
	for (int i = 0; i < length; i++){
		int first = integers[i];
		int second = integers[length-1-i];
		if(first != second){
			return false;
		}
	}
	return true;
}


int sum_elements(int integers[], int length){
	int sum = 0;
	for (int i = 0; i < length; i++){
		sum += integers[i];
	}
	return sum;
}