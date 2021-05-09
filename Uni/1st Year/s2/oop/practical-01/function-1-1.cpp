// function to ...
int sum_array(int array[], int n)
{
	// your code goes here
	int sum = 0;

	for (int i=0; i<n;i++){
		sum=array[i]+sum;
	}
	return sum;
}