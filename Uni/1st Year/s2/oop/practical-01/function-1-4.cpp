// function to ...
int sumtwo(int array[], int secondarray[], int n)
{
	// your code goes hereint 
	int sum = 0;

	for (int i = 0; i<n; i++) {
		sum = sum + array[i] + secondarray[i];
	}
	return sum;
}
