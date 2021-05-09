extern int multiply_if_threes(int integers[], int length);
extern bool is_ascending_multiple_threes(int integers[], int length);
extern int multiply_elements(int integers[],int length);

int multiply_if_threes(int integers[], int length){

	// checking to see if legitimate array
	if (length < 1){
		return -1;
	}

	// declairing value
	int value = 0;

	if (is_ascending_multiple_threes(integers,length) == true){

		// multiplies elements of array if divisible by 3 and 3 apart from eachother
		value = multiply_elements(integers,length);
		return value;

	// returning -2 if not divisible by three or 3 apart
	} else {
		return -2;
	}
}

bool is_ascending_multiple_threes(int integers[], int length){
	// checking to see if legitimate array
	if (length < 1){
		return -false;
	}

	for (int i = 0; i < length; i++){

		// checking to see if divisible by three
		if (integers[i]%3 == 0){

			// checking to see if three apart
			if (i != 0){
				if (integers[i-1] - integers[i] != -3){

					// returning false is not three apart
					return false;
				}
			}

		// returning false if not divisible by three
		} else {
			return false;
		}
	}
	//if pass both tests:
	return true;
}

int multiply_elements(int integers[],int length){
	
	// checking to see if legitimate array
	if (length < 1){
		return -1;
	}

	// multiplying array elements
	int product = 1;
	for (int i = 0; i < length; i++){
		product = product * integers[i];
	}
	return product;
}