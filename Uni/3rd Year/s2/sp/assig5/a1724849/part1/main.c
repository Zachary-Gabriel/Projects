#include <stdio.h>
#include <stdlib.h>
#include "bn.h"

int main(int argc, char const *argv[])
{
	char buf[1000];
	bn_t result = bn_alloc();
	bn_t base = bn_alloc();
	bn_t exp = bn_alloc();
	bn_t modulus = bn_alloc();
	
	bn_fromString(result, "0");
	bn_fromString(base, "2");
	bn_fromString(exp, "5897680552");
	bn_fromString(modulus, "5897680553");
	
	bn_modexp (result, base, exp, modulus);
	bn_toString (result, buf, sizeof(buf));
	printf ("%s\n", buf);

	// fibonacci
	// char buf[1000];
	// bn_t a = bn_alloc();
	// bn_t b = bn_alloc();

	// bn_fromString(a, "0");
	// bn_fromString(b, "1");
	
	// for (int i = 0; i < 100; i++) 
	// {
	// 	bn_toString(a, buf, sizeof(buf));
	// 	printf("%2d: %s\n", i, buf);
	// 	bn_add(a, a, b);
	// 	bn_t t = a;
	// 	a = b;
	// 	b = t;
	// }

	// char bufa[1000];
	// char bufb[1000];
	// bn_t a = bn_alloc();
	// bn_t b = bn_alloc();
	// bn_fromString(a, "");
	// bn_fromString(b, "");
	// bn_toString(a, bufa, sizeof(bufa));
	// printf("%s\n", bufa);
	// bn_mul (a, a, a);
	// bn_toString(a, bufb, sizeof(bufb));
	// printf("%s\n", bufb);
	return 0;
}