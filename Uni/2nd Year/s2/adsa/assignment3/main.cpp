#include <iostream>
#include <vector>
#include <string>

using namespace std;
// to keep track of the array size
static int size = 0;
vector<string> del_value (vector<string> table, string deleting)
{
	// Converting the last character into an index for the table 
	char key = deleting[deleting.size() -1];
	int index = int (key) - 97;
	
	int counter = 0;
	while ( !( table[index] == "" || table[index] == deleting ) )
	{
		if ( counter == 26) 
		{
			return table;
		}	
		// so that we arent locked in here forever
		++ counter ; 
		// so we don't run into seg faults
		++ index ;
		if (index == 26)
		{
			index = 0;
		}
	}

	// if no element exists to delete
	if (table[index] == "") 
	{
		return table;
	}
	// if in table, delete element;
	--size;
	table[index] = "";
	return table;
}

vector<string> add_value (vector<string> table, string adding)
{
	// Converting the last character into an index for the table 
	char key = adding[adding.size() -1];
	int index = int (key) - 97;
	if (size == 26)
	{
		return table;
	}

	int counter = 0;
	while ( ! ( table[index] == "" || table[index] == adding ) )
	{
		// if already in table or if no space in table
		if (table[index] == adding || counter == 26) 
		{
			return table;
		}
		// so that we arent locked in here forever
		++ counter ; 
		// so we don't run into seg faults
		++ index ;
		if (index == 26)
		{
			index = 0;
		}
	}

	// if already in table
	if (table[index] == adding) 
	{
		return table;
	}
	// if not in table, add to table
	++size;
	table[index] = adding;
	return table;
}
int main(void)
{
	// getting the input line
	string input = "";
	getline(cin,input);
	vector<string> table (26);

	// getting variables
	string temp = "";
	bool isA = false;
	int length = input.size();
	// parsing input into the things we want 
	for (int i = 0; i < length; ++i)
	{
		if (input[i] == 'A')
		{
			isA = true;
		}
		else if (input[i] == 'D')
		{
			isA = false;
		}
		else if (input[i] != ' ')
		{
			temp += input[i];
		}
		else if (input[i] == ' ')
		{
			if (isA)
			{
				table = add_value (table, temp);
			}
			if (!isA)
			{
				table = del_value (table, temp) ;
			}
			temp = "";
			isA = false;
		}
	}
	// for the last word (shouldnt be a problem due to the way its handled)
	if (isA)
	{
		table = add_value (table, temp);
	}
	else
	{
		table = del_value (table, temp) ;
	}
	//priting table
	for (int i = 0; i < 26; ++i)
	{
		if (table[i] != "")
		{
			cout << table[i] << ' ' ;
		}
	}
	cout << endl;
	return 0;
}