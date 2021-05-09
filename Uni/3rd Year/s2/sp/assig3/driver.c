#include "spalloc.h"
#include <stdio.h>
#include <stdbool.h>

const int input_buffer = 100;

/* parsing input */
bool input_parser (char* input, uint32_t* seed)
{
	/* initialising variables */
	int i = 0;
	int digit;
	*seed = 0;

	/* while there's character's left in the string, append them to the number */
	while (input[i] != '\0' && i < input_buffer && input[i] != '\n')
	{
		digit = (int) (input[i]);
		
		/* for bad input */
		if (digit < 48 || digit > 59)
		{
			return false;			
		}

		/* appending to seed number and iterating along the string */
		*seed = *seed * 10 + digit - 48;
		++ i;
	}
	
	return true;
}

void* operation (uint32_t rand_num, void* ptr1, void* ptr2)
{
	/* executing operation based on least significant bits */
	if (rand_num%4 == 0)
	{
		printf ("malloc\n");
		ptr1 = sp_malloc (rand_num);
	}
	else if (rand_num%4 == 1)
	{
		printf ("calloc\n");
		ptr1 = sp_calloc(1, rand_num);
	}
	else if (rand_num%4 == 2)
	{
		printf ("realloc\n");
		// ptr1 = sp_realloc (ptr2, rand_num);
	}
	else
	{
		printf ("free\n");
		sp_free (ptr1);
	}
	printf ("done\n");
	return ptr1;
}

void rand_driver ()
{
	uint32_t rand_num;
	int op = 0;
	void* a = NULL;
	void* b = NULL;
	void* c = NULL;
	while (true)
	{
		rand_num = sp_rand ();
		printf ("a: %u\n", a);
		printf ("b: %u\n", b);
		printf ("c: %u\n", c);
		printf ("number: %d\n", op);
		op++;
		switch (rand_num%27)
		{
			/* a as the focal pointer */
			case 0:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =a,b, size:%d\n", rand_num);
				a = operation (rand_num, a, b);
				break;
			
			case 1:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =a,a, size:%d\n", rand_num);
				a = operation (rand_num, a, a);
				break;
			
			case 2:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =a,c, size:%d\n", rand_num);
				a = operation (rand_num, a, c);
				break;
			
			case 3:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b = a,b, size:%d\n", rand_num);
				b = operation (rand_num, a, b);
				break;
			
			case 4:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =a,a, size:%d\n", rand_num);
				b = operation (rand_num, a, a);
				break;
			
			case 5:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =a,c, size:%d\n", rand_num);
				b = operation (rand_num, a, c);
				break;
			
			case 6:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =a,b, size:%d\n", rand_num);
				c = operation (rand_num, a, b);
				break;
			
			case 7:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =a,a, size:%d\n", rand_num);
				c = operation (rand_num, a, a);
				break;
			
			case 8:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =a,c, size:%d\n", rand_num);
				c = operation (rand_num, a, c);
				break;
			
			/* b as the focal pointer */
			case 9:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =b,a, size:%d\n", rand_num);
				a = operation (rand_num, b, a);
				break;
			
			case 10:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =b,b, size:%d\n", rand_num);
				a = operation (rand_num, b, b);
				break;
			
			case 11:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =b,c, size:%d\n", rand_num);
				a = operation (rand_num, b, c);
				break;
			
			case 12:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =b,a, size:%d\n", rand_num);
				b = operation (rand_num, b, a);
				break;
			
			case 13:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =b,b, size:%d\n", rand_num);
				b = operation (rand_num, b, b);
				break;
			
			case 14:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =b,c, size:%d\n", rand_num);
				b = operation (rand_num, b, c);
				break;
			
			case 15:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =b,a, size:%d\n", rand_num);
				c = operation (rand_num, b, a);
				break;
			
			case 16:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =b,b, size:%d\n", rand_num);
				c = operation (rand_num, b, b);
				break;
			
			case 17:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =b,c, size:%d\n", rand_num);
				c = operation (rand_num, b, c);
				break;
			
			/* c as the focal pointer */
			case 18:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =c,a, size:%d\n", rand_num);
				a = operation (rand_num, c, a);
				break;
			
			case 19:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =c,b, size:%d\n", rand_num);
				a = operation (rand_num, c, b);
				break;
			
			case 20:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("a =c,c, size:%d\n", rand_num);
				a = operation (rand_num, c, c);
				break;
			
			case 21:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =c,a, size:%d\n", rand_num);
				b = operation (rand_num, c, a);
				break;
			
			case 22:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =c,b, size:%d\n", rand_num);
				b = operation (rand_num, c, b);
				break;
			
			case 23:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("b =c,c, size:%d\n", rand_num);
				b = operation (rand_num, c, c);
				break;
			
			case 24:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =c,a, size:%d\n", rand_num);
				c = operation (rand_num, c, a);
				break;
			
			case 25:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =c,b, size:%d\n", rand_num);
				c = operation (rand_num, c, b);
				break;
			
			case 26:
				rand_num = sp_rand() %8192; /* to not run out of space */
				printf ("c =c,c, size:%d\n", rand_num);
				c = operation (rand_num, c, c);
				break;
		}
	}
}

int main(int argc, char const *argv[])
{
	/* initialising variables */
	uint32_t seed;
	char input[input_buffer];

	/* getting input */
	fgets (input, input_buffer, stdin);
	
	/* parsing input */
	bool is_number = input_parser (input, &seed);

	/* making random number using input seed */
	if (is_number)
	{
		rand_driver ();
	}
	/* fuzzing from AFL */
	else
	{
		/* printing output */
		printf ("fuzzer: parsing not implemented\n");
	}	
	return 0;
}