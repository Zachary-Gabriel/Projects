#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

int power (int a, int n)
{
	if (n > 0)
	{
		return a*power(a,n-1);
	}
	return 1;
}

std::vector<int> add (std::vector<int> first, std::vector<int> second, int base)
{
	// Preallocating memory
	std::vector<int> result;
	int value1, value2, difference, temp, i = 0, carry = 0, length1 = first.size(), length2 = second.size();

	// making first larger than second
	if (length1 < length2)
	{
		result = first;
		first = second;
		second = result;

		length1 = length2;
		length2 = second.size();
		result.clear();
	}
	difference = length1-length2;

	// adding the two numbers
	for (i = length2-1; i >= 0; i--)
	{
		value1 = first.at (i + difference);
		value2 = second.at (i);
		temp = value1 + value2 + carry;

		// if there should be a carry
		if ( (temp) >= base)
		{
			temp -= base;
			result.push_back (temp);
			carry = 1;
		}
		else
		// if there shouldn't be a carry
		{
			result.push_back (temp);
			carry = 0;
		}
	}

	// if two numbers are equal in length
	if (difference == 0)
	{
		// if there's a carry at the end of the number
		if (carry == 1)
		{
			result.push_back (1);
		}
		std::reverse (result.begin(),result.end());
	}

	// if one is strictly larger than the other
	else
	{
		for (i = difference-1; i >=0; i--)
		{
			value1 = first.at(i);
			temp = value1 + carry;
			if (temp >= base)
			{
				temp -= base;
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			result.push_back (temp);
		}
		// if there's a carry at the end of the number
		if (carry == 1)
		{
			result.push_back (1);
		}
		std::reverse (result.begin(),result.end());
	}
	return result;
}

std::vector<int> subtract (std::vector<int> first, std::vector<int> second, int base)
{
	// Preallocating memory
	std::vector<int> result;
	int value1, value2, temp, i = 0, carry = 0, length1 = first.size(), length2 = second.size(), difference = length1-length2;

	// subtracting 2 digits
	for (i = length2-1; i >= 0; i--)
	{
		value1 = first.at (i + difference);
		value2 = second.at (i);
		temp = value1 - value2 - carry;

		// if there should be a carry
		if (temp < 0)
		{
			temp += base;
			result.push_back (temp);
			carry = 1;
		}
		else
		// if there shouldn't be a carry
		{
			result.push_back (temp);
			carry = 0;
		}
	}

	// if two numbers are equal in length
	if (difference == 0)
	{
		std::reverse (result.begin(),result.end());
	}

	// if one is strictly larger than two (in vector length)
	else
	{
		for (i = difference-1; i >=0; i--)
		{
			value1 = first.at(i);
			temp = value1 - carry;
			if (temp >= base)
			{
				temp += base;
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			result.push_back (temp);
		}
		std::reverse (result.begin(),result.end());
	}
	return result;
}

std::vector<int> multiply (std::vector<int> first, std::vector<int> second, int base)
{
	std::vector<int> result;
	int  i, length1 = first.size(), length2 = second.size();

	// making first larger than second
	if (length1 < length2)
	{
		result = first;
		first = second;
		second = result;

		length1 = length2;
		length2 = second.size();
		result.clear();
	}
	// primary school method
	if (length2 < 4)
	{
		//preallocating memory
		int carry = 0, value1, value2 = 0, multiple, remainder;

		// finding the integer value of the number that is less than 1000 (in any base)
		for (i = 0; i < length2; i++)
		{
			value2 = base*value2 + second[i];
		}

		// looping through the two numbers
		for (i = length1-1; i >= 0; i--)
		{
			value1 = first[i];
			multiple = value1 * value2 + carry;
			remainder = multiple%base;
			result.push_back(remainder);
			carry = (multiple-remainder)/base;
		}

		// adding the rmaining carry-on values at the end
		while (carry!=0)
		{
			remainder = carry%base;
			result.push_back(remainder);
			carry = (carry-remainder)/base;
		}
		std::reverse (result.begin(),result.end());
		return result;
	}
	else // Kartsuba Algorithm
	{
		// allocating memory
		std::vector<int> result, p0, p1, p2, a0, a1, b0, b1, temp1, temp2;

		// splitting up a into a0 and a1
		for (i = 0; i < length1; i++)
		{
			if (i < floor(length1/2))
			{
				a1.push_back(first[i]);
			}
			else
			{
				a0.push_back(first[i]);
			}
		}
		// splitting up b into b0 and b1
		for (i = 0; i < length2; i++)
		{
			if (i < floor(length2/2))
			{
				b1.push_back(second[i]);
			}
			else
			{
				b0.push_back(second[i]);
			}
		}

		// algorithm
		p2 = multiply (a1, b1, base);
		p0 = multiply (a0, b0, base);
		temp1 = add (a1, a0, base);
		temp2 = add (b1, b0, base);
		p1 = multiply (temp1, temp2, base);

		//freeing memory
		a0.clear();
		a1.clear();
		b0.clear();
		b1.clear();
		temp1.clear();
		temp2.clear();

		temp1 = add (p0,p2, base);
		temp2 = subtract (p1,temp1,base);
		temp1.clear();

		// padding with 0's
		for (i = 0; i < (length1+length2)/2; i++)
		{
			if (i <(length1+length2)/4)
			{
				temp2.push_back(0);
			}
			p2.push_back(0);
		}
		temp1 = add (temp2,p2, base);
		result = add (p0, temp1, base);

		// free up memory
		temp1.clear();
		temp2.clear();
		p0.clear();
		p1.clear();
		p2.clear();
		return result;
	}
}

int main (void) 
{
	// Preallocating memory
	std::vector<int> first, second;
	std::string input1, input2, input3;
	int base = 0, thisInt;
	unsigned i = 0, length1, length2;
	//input
	std::cin >> input1 >> input2 >> input3;

	length1 = input1.size();
	length2 = input2.size();
	// converting inputs strings to vectors and ints
	for (i = 0; i < length1; i++)
	{
		thisInt = int (input1.at(i))-48;
		first.push_back(thisInt);
	}
	
	for (i = 0; i < length2; i++)
	{
		thisInt = int (input2.at(i))-48;
		second.push_back(thisInt);
	}
	for (i = 0; i < input3.size(); i++)
	{
		thisInt = int (input3.at(i))-48;
		base = 10*base + thisInt;
	}
	// running add function
	std::vector<int> v = add (first, second, base);
	for (i = 0; i < v.size(); i++)
	{
		std::cout << v[i];
	}
	std::cout << ' ';

	//running karatsuba algorithm 
	v = multiply (first, second, base);
	for (i = 0; i < v.size(); i++)
	{
		std::cout << v[i];
	}	
	std::cout << ' ' << '0' << std::endl;
	return 0;
}