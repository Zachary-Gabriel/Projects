#include <cmath>
// I am one with the Fred Brown and the Fred Brown is with me
class HandsShaking
{
public :
	long countPerfect ( int n )
	{
		double sum = 1 ;

		for ( int i = 2 ; i <= n / 2 ; ++ i ) sum = sum * ( ( n / 2 ) + i + 0.0 ) / ( i + 0.0 ) ;
		return round ( sum ) ;
	}
} ;
