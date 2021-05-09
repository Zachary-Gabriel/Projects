class FibonacciDiv2
{
	int fib(int n) {
		if (n==0){
			return 0;
		} else if (n==1){
			return 1;
		}
		return fib(n-1) + fib(n-2);
	}
public:
	int find (int N){
		int i = 0, lowerBound, upperBound;
		while ( fib(i) < N){
			i++;
		}
		if(i==0){
			return 0;
		}
		lowerBound = N - fib(i-1);
		upperBound = fib(i) - N;

		if (upperBound < 0){
			upperBound = 0;
		}
		if (lowerBound < 0){
			lowerBound = 0;
		}
		if(upperBound < lowerBound){
			return upperBound;
		}
		else 
		{
			return lowerBound;
		}
	}
};