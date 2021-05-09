#include <stdio.h>
#include <stdlib.h>
#include "bn.h"

int main(int argc, char const *argv[])
{
	char buf[1000];
	bn_t a = bn_alloc();
	bn_t b = bn_alloc();
	
	bn_fromString(a, "0");
	bn_fromString(b, "-1001234");
	for (int i = 0; i < 7; ++ i)
	{
		printf("%u\n", (int) b-> bn_data[i]);
	}

	// for (int i = 0; i < 100; i++) 
	// {
	// 	bn_toString(a, buf, sizeof(buf));
	// 	printf("%2d: %s\n", i, buf);
	// 	bn_add(a, a, b);
	// 	bn_t t = a;
	// 	a = b;
	// 	b = t;
	// }
	return 0;
}