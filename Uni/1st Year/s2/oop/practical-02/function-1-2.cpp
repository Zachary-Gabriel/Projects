int identity(int array[10][10]){
	int count = 0;

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			if(i == j && array[i][j] == 1){
				count++;
			}

			if (i != j && array[i][j] == 0){
				count++;
			}
		}
	}

	if (count == 100){
		return 1;
	} else {
		return 0;
	}
}