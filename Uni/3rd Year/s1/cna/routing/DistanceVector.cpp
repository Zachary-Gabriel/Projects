#include <iostream> // strings and cout
#include <fstream> // for input file
#include <limits> // for the max int
#include <vector> // for vectors
#include <algorithm> // for sorting

using namespace std;

// data structure for this assignment
struct Router
{
public:
	int** table; // stores the routing table such that the first index is the destination and the second / inner index is the middle man
	vector<int> nei; // stores the index of the neighbors of this router - declared in #START t=0
	int* optimal; // an array which stores the lowest value from i->j
	string* optimal_name; // an array which stores the router's ID with the lowest value from i->j
};

// frees memory for good practice
void free_memory(Router& network, int node_num)
{
	// deleting the network edges
	for (int i = 0; i < node_num; ++i)
	{
		delete[] network.table[i];
	}
	delete[] network.table;
	delete[] network.optimal;
	delete[] network.optimal_name;
	network.nei.clear();

	return ;
}

// printing the variables for the purpose of debugging
void print_variables_debug (int& node_num, string*& node_names, Router* network)
{
	// printing non-network variables
	cout << endl << "Printing the network: " << endl << "node number: " << node_num << endl << "node names: ";
	for (int i = 0; i < node_num; ++i)
	{
		cout << node_names[i] << ", ";
	}
	cout << endl << endl << "Network: ";

	// printing the network
	for (int i = 0; i < node_num; ++i)
	{
		cout << endl << "Router: " << node_names[i] << endl;
		for (int j = 0; j < node_num; ++j)
		{
			for (int k = 0; k < node_num; ++k)
			{
				if (i != j && i != k) // because the router's table size is one greater than it should be to make coding easier
				{
					if (network[i].table[j][k] == -1) // if table spot is empty - a value DNE
					{
						cout << " INF ";
					}
					else
					{
						cout <<  " " << network[i].table[j][k] << " ";	// printing the value if it does exist
					}
				}
			}
			if (i != j)
				cout << endl; // next row of the table
		}
		cout << "neighbors: ";
		for (int j = 0; j < network[i].nei.size(); ++j) // printing the neighbors array
		{
			if (j == 0)
				cout << node_names[network[i].nei[j]];	
			else
				cout << ", " << node_names[network[i].nei[j]];	
		}
		cout << endl;
		
	}
	
	for (int i = 0; i < node_num; ++i)
	{
		// printing the incex of the neighbors
		cout <<endl<<  node_names[i] << " optimal:" << endl;
		for (int j = 0; j < node_num; ++j)
		{
			cout << network[i].optimal[j] << " ";
		}
		// printing the names of the neighbors
		cout << endl;
		for (int j = 0; j < node_num; ++j)
		{
			cout << network[i].optimal_name[j] << " ";
		}
	}
	cout << endl;
	return ;
}

// Converts the character inputs to integers
void parse_inputs_int (char input, int& variable)
{
	if (48 <= int(input) && int(input) <= 57) // if character is a number (ascii 48 - 57)
	{
		variable = (variable * 10) + int(input) - 48;
	}
	return ;
}

// Returns the index of the array with comparing a string against the array
int find_name_index (string* node_names, string compare)
{
	int i = 0;
	while (node_names[i] != compare)
	{
		++i;
	}
	return i;
}

// Parses the input of the first number representing number of nodes/ routers
void parse_inputs_node_num (char input, int& node_num, string*& node_names, int& phase)
{
	int i = 0;
	parse_inputs_int (input, node_num);
	if (input == '\n' || input == ' ')
	{
		node_names = new string[node_num];
		for (i = 0; i < node_num; ++i)
		{
			node_names[i] = "";
		}
		i = 0;
		++ phase;
	}
	return ;
}

// Parses the node names using the input (only a->z or A->Z characters are allowed in the name)
void parse_inputs_node_names (char input, int& node_num, string*& node_names, Router*& network, int& phase, int& i)
{
	if (input == '\n') // new line -> next phase or name
	{
		++ i;
		if (i >= node_num)
		{
			++ phase;
			i = 0;
			sort (node_names, node_names + node_num);

			network = new Router[node_num];
			for (int i = 0; i < node_num; ++i)
			{
				network[i].table = new int*[node_num];
				network[i].optimal = new int[node_num]; // lowest values in k excluding -1
				network[i].optimal_name = new string[node_num]; // lowest values in k excluding -1
				for (int j = 0; j < node_num; ++j)
				{
					network[i].table[j] = new int[node_num];
					for (int k = 0; k < node_num; ++k)
					{
						network[i].table[j][k] = -1;
					}
				}
			}
		}
	}

	// A->Z, a->z
	else if ((65<= int(input) && int(input) <= 90) || (97 <= int(input) && int(input) <= 122))
	{
		node_names[i] += input;
	}
	return ;
}

// Parses the input of the second number representing number of edges
void parse_inputs_edge_num (char input, int& edge_num, int& phase)
{
	parse_inputs_int (input, edge_num);
	if (input == '\n')
	{
		++ phase;
	}
	
	return ;
}

// going through each line of the edge part of the input (the last part of the original and changed files)
void parse_inputs_edges (char input, int& node_num, string*& node_names, int& edge_num, Router*& network, int& phase, ifstream& ifs, bool first_time)
{
	int i = 0;
	int j = 0;
	for (int k = 0; k < edge_num; ++k)
	{
		string temp_name = "";
		while (!(65<= int(input) && int(input) <= 90) || (97 <= int(input) && int(input) <= 122))
		{
			input = ifs.get();
		}

		while ((65<= int(input) && int(input) <= 90) || (97 <= int(input) && int(input) <= 122))
		{
			temp_name += input;
			input = ifs.get();
		}
		i = find_name_index (node_names, temp_name);
		input = ifs.get();
		temp_name = "";

		while (!(65<= int(input) && int(input) <= 90) || (97 <= int(input) && int(input) <= 122))
		{
			input = ifs.get();
		}

		while ((65<= int(input) && int(input) <= 90) || (97 <= int(input) && int(input) <= 122))
		{
			temp_name += input;
			input = ifs.get();
		}

		j = find_name_index (node_names, temp_name);
		input = ifs.get();
		network[j].table[i][i] = 0;
		network[i].table[j][j] = 0;

		while (48 <= int(input) && int(input) <= 57 && ifs.good())
		{
			parse_inputs_int(input, network[j].table[i][i]);
			parse_inputs_int(input, network[i].table[j][j]);

			input = ifs.get();
		}

		// remembering which nodes were the neighbors (as no new edges are made in the second iteration)
		if (first_time)
		{
			network[i].nei.push_back (j);
			network[j].nei.push_back (i);
		}

		// setting default optimal edges to the starting ones
		cout << "t=0 distance from " << node_names[i] << " to " << node_names[j] << " via " << node_names[j] << " is " << network[i].table[j][j] << endl;
		cout << "t=0 distance from " << node_names[j] << " to " << node_names[i] << " via " << node_names[i] << " is " << network[j].table[i][i] << endl;

	}
	++ phase;
	return ;
}

// Parsing the input file into variables
void parse_inputs (char input, int& node_num, string*& node_names, int& edge_num, Router*& network, int& phase, int& i, ifstream& ifs, bool first_time)
{
	switch (phase)
	{
		// Getting node number (phase 1)
		case 0:	
		parse_inputs_node_num (input, node_num, node_names, phase);
		break;

		// Getting node names (phase 2)
		case 1:
		parse_inputs_node_names (input, node_num, node_names, network, phase, i);
		break;

		// Getting edge number (phase 3)
		case 2:
		parse_inputs_edge_num (input, edge_num, phase);
		break;

		// Getting edges (phase 4)
		case 3:
		parse_inputs_edges (input, node_num, node_names, edge_num, network, phase, ifs, first_time);
		break;	
	}
	
	return ;
}

// reading the first argument and putting it into variables
void parse_init_variables (int& node_num, string*& node_names, int& edge_num, Router*& network, char const *argv)
{
	// initialising variables
	int phase = 0;
	int i = 0;
	char input;
	ifstream ifs;

	// Parsing input into variables
	ifs.open (argv, ifstream::in);

	// Running through the input character by character
	while (ifs.good() && !ifs.eof()) 
	{
		input = ifs.get();
		parse_inputs (input, node_num, node_names, edge_num, network, phase, i, ifs, true);
	}

	// closing the input file
	ifs.close();
	return ;
}

// reading the second argument and updating the network
void parse_update_input (int& node_num, string*& node_names, int& edge_num, Router*& network, char const *argv)
{
	// initialising variables
	int phase = 2; // starting later
	int i = 0;
	char input;
	ifstream ifs;
	edge_num = 0; // resetting

	// Parsing input into variables
	ifs.open (argv, ifstream::in);

	// Running through the input character by character
	while (ifs.good() && !ifs.eof()) 
	{
		input = ifs.get();
		parse_inputs (input, node_num, node_names, edge_num, network, phase, i, ifs, false);
	}

	// closing the input file
	ifs.close();
	return ;
}

// creates a duplicate router
void create_duplicate (Router original, Router& duplicate, int node_num)
{	
	// allocating space and setting to zero
	duplicate.optimal = new int[node_num];
	duplicate.optimal_name = new string[node_num];
	duplicate.table = new int*[node_num];
	duplicate.nei.clear();

	// copying over the neighbors
	for (int i = 0; i < original.nei.size (); ++i)
		duplicate.nei.push_back(original.nei[i]);

	// copying over the array values
	for (int i = 0; i < node_num; ++i)
	{
		duplicate.table[i] = new int[node_num];
		duplicate.optimal[i] = original.optimal[i];
		duplicate.optimal_name[i] = original.optimal_name[i];

		// copying over the matrix values
		for (int j = 0; j < node_num; ++j)
			duplicate.table[i][j] = original.table[i][j];
	}
	return ;
}

// fixing optimal array to be correct
void fix_optimal_array (Router& router, int node_num, string* node_names)
{
	for (int i = 0; i < node_num; ++i)
	{
		// initialising variables
		int min = numeric_limits<int>::max (); // largest number in c++
		int index = 0;

		// looping through matrix - finding smallest value for router x-> y through all known routes
		for (int j = 0; j < node_num; ++j)
		{
			// if value exists and is the smallest thus far
			if (router.table[i][j] < min && router.table[i][j] != -1)
			{
				min = router.table[i][j];
				index = j;
			}
		}

		// using -1 to signify INF because realistically, there are no negative weights
		// and using the largest int in c++ will lead to overflow errors
		if (min == numeric_limits<int>::max ())
		{
			min = -1;
		}

		// saving the changes to memory
		router.optimal[i] = min;
		router.optimal_name[i] = node_names[index];
	}
	return;
}
void DV (Router*& network, Router*& duplicate, string* node_names, int node_num, int i, int j, int& time)
{
	// initialising variables
	int index = network[i].nei[j];
	int min = network[i].optimal[index];
	int mindex = i; // min index - default set to router - for debugging mostly

	// work around for INF
	if (min == -1)
		min = numeric_limits<int>::max ();

	// checking for new smallest edges for both the router's table and optimal table
	for (int k = 0; k < node_num; ++k)
	{
		// if there exists a new smallest path from router i to router j through router k
		if (network[index].optimal[k] != -1 && i != k)
		{
			// if new edge of router i to router j through router k has been found, store it
			network[i].table[k][index] = network[i].table[index][index] + network[index].optimal[k];

			// if new edge is found, print to console
			if (network[i].table[k][index] != duplicate[i].table[k][index])
			{
				duplicate[i].table[k][index] = network[i].table[k][index];
				cout << "t="<< time <<" distance from "<< node_names[i] <<" to "<< node_names[k] << " via "<< node_names[index] << " is " << network[i].table[k][index] << endl;
			}

			// if new optimal edge of router i to router j (regardless of k) has been found
			if (network[i].table[index][index] + network[index].optimal[k] < min)
			{
				min = network[i].table[index][index] + network[index].optimal[k];
				mindex = k;
			}	
		}
	}

	// if is INF
	if (min == numeric_limits<int>::max ())
	{
		network[i].optimal[index] = i;
		network[i].optimal_name[index] = node_names[i];
	}

	// if there is a min value that has been set
	else
	{
		network[i].optimal[index] = min;
		network[i].optimal_name[index] = node_names[mindex];
	}
	return;
}

// working through the network so that it has the most optimal edges
void shuffle (int node_num, string* node_names, Router*& network)
{
	// initialising variables
	bool change = true;
	int time = 0;
	bool press_enter = false;

	// setting the optimal value before we do math to it
	for (int i = 0; i < node_num; ++i)
	{
		sort(network[i].nei.begin(), network[i].nei.end());
		fix_optimal_array (network[i], node_num, node_names);
	}
	
	// looping through until there are no changes to the optimal values (lowest cost edges)
	while (change)
	{
		// initialising variables
		change = false;
		++time;
		Router* duplicate = new Router[node_num];
		cout << endl;

		// looping through each router in the network
		for (int i = 0; i < node_num; ++i)
		{
			// creating a duplicate router to remember if change has occured
			create_duplicate (network[i], duplicate[i], node_num);	

			// look for changes
			for (int j = 0; j < network[i].nei.size(); ++j)
			{
				DV (network, duplicate, node_names, node_num, i, j, time);
			}
		}

		// checking for changes and freeing memory of the duplicate routers
		for (int i = 0; i < node_num; ++i)
		{
			fix_optimal_array (network[i], node_num, node_names);
			for (int j = 0; j < node_num; ++j)
			{
				if (duplicate[i].optimal[j] != network[i].optimal[j])
				{
					change = true;
				}
			}
			free_memory (duplicate[i], node_num);
		}
		delete[] duplicate;
		
	}
	return ;
}

// printing the linking tables
void printing_optimal (int node_num, string* node_names, Router*& network)
{
	for (int i = 0; i < node_num; ++i)
	{
		int index = i;
		for (int j = 0; j < node_num; ++j)
		{
			// for normal values
			if (network[i].optimal[j] != -1 && i!= j)
			{	
				cout << "router "<< node_names[i] <<": "<< node_names[j] <<" is "<< network[i].optimal[j] <<" routing through " << network[i].optimal_name[j] << endl;
			}
			// if the poop has hit the fan (for DV)
			else if (network[i].optimal[j] == -1 && i!= j)
			{
				cout << "router "<< node_names[i] <<": "<< node_names[j] <<" is INF routing through " << network[i].optimal_name[j] << endl;
			}
		}
		cout << endl;
	}
	return ;
}

// creates distance vector
int main(int argc, char const *argv[])
{
	// bad execution
	if (argc != 3)
	{
		cout << "Bad or missing arguments" << endl;
		return 0;
	}

	// initialising variables
	string* node_names;
	Router* network;
	int node_num = 0;
	int edge_num = 0;

	// converting the input text file into the variables
	cout << "#START" << endl << endl;
	parse_init_variables (node_num, node_names, edge_num, network, argv[1]);

	// keeps shuffling through the routers for t= 1 onward
	shuffle (node_num, node_names, network);

	// displaying the optimal arrays for each router in the network
	cout << "#INITIAL" << endl << endl;
	printing_optimal (node_num, node_names, network);
	
	// converts the second argument then shuffles through for t= 1 onward
	cout << "#UPDATE" << endl;
	parse_update_input (node_num, node_names, edge_num, network, argv[2]);
	shuffle (node_num, node_names, network);
	
	// displaying the optimal arrays for each router in the network
	cout << "#FINAL" << endl;
	printing_optimal (node_num, node_names, network);

	// Freeing memory
	for (int i = 0; i < node_num; ++i)
	{
		free_memory (network[i], node_num);
	}
	delete[] network;
	delete[] node_names;
	
	return 0;
}