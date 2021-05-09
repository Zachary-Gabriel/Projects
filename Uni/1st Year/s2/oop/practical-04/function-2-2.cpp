int maximum_sum(int *nums,int length){
	int max_number = 0;
	if(length == 1){
		if( nums[0]>0){
			return nums[0];
		} else {
			return 0;
		}
	}

	for(int i = 0; i < length; i++){
		for (int j = 0; j < i; j++){

			int sum = 0;
			for (int k = j; k < i; k++){
				sum += nums[k]; 
			}
			if (sum >= max_number){
				max_number = sum;
			}
		}
	}
	return max_number;
}