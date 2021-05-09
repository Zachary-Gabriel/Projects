/*
created by: Boon Aun Lim (Moses), Zachary Gabriel
ID: a1757792, a1724849
time: 19/10/2020
Contact Email: a1757792@student.adelaide.edu.au, a1724849@student.adelaide.edu.au
Include int main(int argc,char *argv[])
input:  input.txt page_size frames algorithm algorithm_args
output: Screen
*/

#include <iostream> // input / output
#include <vector> // vector ds
#include <fstream> // file stream for input file parsing
#include <iomanip> // for printing outputs in hex and dec
#include <bits/stdc++.h> // queue and unorder sets
#include <sstream>

using namespace std;

// the output variables
int events = 0;
int reads = 0;
int writes = 0;
int page_faults = 0;

// input variables
vector <u_int64_t> addresses;
vector <bool> is_read;
int page_size = 0;
int frame_num = 0;

// returns base rasied to the power of pow
uint power (int base, int pow)
{
	if (pow == 0)
	{
		return 1;
	}

	if (pow == 1)
	{
		return base;
	}
	
	return base * power (base, pow-1);;
}

uint log_base_2 (uint val)
{
	uint x = 1;
	uint ret = 0;

	while (val > x)
	{
		ret ++;
		x *= 2;
	}

	return ret;
}

// converts a string to an int
int string_to_int(string a)
{
	int ret = 0;
	int i = 0;
	while (a[i] != '\0' && a[i] != '\n' && a[i] != ' ' && a[i] != 13)
	{
		if ('0' <= a[i] && a[i] <= '9')
		{
			ret = ret * 10 + a[i] - '0';
		}
		++i;
	}
	return ret;
}

int bitOffset(int pageSize)
{
	int tempPageSize = 0;
	int bits = 0;

	while (tempPageSize < pageSize)
	{
		bits += 1;
		tempPageSize = pow(2, bits);
	}

	return bits;
}

// when the frame is entirely filled, overwrite the oldest
// address in the frame
void fifo ()
{
	// represent pages currently in the frame
	unordered_set <int> inFrame;
	// store pages in FIFO order
	queue <int> index;
	// store pages that are "dirty"
	vector <int> dirty;

	int offset = bitOffset(page_size);

	for (int i = 0; i < addresses.size(); i++)
	{
		int pageNumber = addresses[i] >> offset;
		if (is_read[i] == false)
		{
			dirty.push_back(pageNumber);
		}

		// sort dirty pages and remove any duplicates
		sort(dirty.begin(), dirty.end());
		dirty.erase(unique(dirty.begin(), dirty.end()), dirty.end());

		// if there is still space in the frame
		if (inFrame.size() < frame_num)
		{	
			// insert page into frame if page is not present
			if (inFrame.find(pageNumber) == inFrame.end())
			{
				// insert current page into frame
				inFrame.insert(pageNumber);
				// increment page fault as page was not in the frame
				page_faults++;
				// push current page into the queue
				index.push(pageNumber);
				// increment reads when address is written into frame
				reads++;
			}
		}
		// if the frame is full
		else
		{
			if (inFrame.find(pageNumber) == inFrame.end())
			{
				// store oldest address in the queue to
				// be used to find and erase address from
				// frame
				int addr = index.front();

				int counter = 0;
				// remove dirty page from vector, becomes clean
				for (const int& i : dirty)
				{
					if (i == addr)
					{
						writes++;
						dirty.erase(dirty.begin() + counter);
						break;
					}
					counter++;
				}

				// remove oldest address from the queue
				index.pop();
				// remove address from frame
				inFrame.erase(addr);
				// insert current page into frame
				inFrame.insert(pageNumber);
				// push current page into queue
				index.push(pageNumber);
				// increment page_faults and reads when 
				// an address is overwritten
				page_faults++;
				reads++;
			}
		}
	}
}

// inserts the page-table-entry to the front of the page table
void push_to_front (uint64_t* page_table, bool* is_dirty, uint64_t pte)
{
	uint64_t* temp = new uint64_t[frame_num] ();
	bool* temp_dirt = new bool[frame_num] ();
	bool ejected = true;
	int pte_index = frame_num;

	// duplicating
	for (int i = 0; i < frame_num; ++i)
	{
		temp[i] = page_table[i];
		temp_dirt[i] = is_dirty[i];
	}

	// hunting down the pte
	for (int i = 0; i < frame_num; ++i)
	{
		if (page_table[i]/ page_size == pte / page_size)
		{
			pte_index = i;
			ejected = false;
			break;
		}
	}

	// shuffling the page table down
	for (int i = 1; i < frame_num; ++i)
	{
		// slide forward until overwritten pte
		if (i < pte_index)
		{
			page_table[i] = temp[i-1];
			is_dirty[i] = temp_dirt[i-1];
		}
		// unchanged
		else if (i > pte_index)
		{
			break;
		}
	}
	
	// if ejecting a dirty
	if (ejected && temp_dirt[frame_num-1])
	{
		// cout <<hex<< "in: " << pte << ", out: " << temp[frame_num-1]<<endl;
		writes ++;
	}
	
	// if rearranging
	if (pte_index == frame_num)
	{
		is_dirty[0] = false;
	}
	else
	{
		is_dirty[0] = temp_dirt[pte_index];
	}
	page_table [0] = pte;

	free (temp);
	free (temp_dirt);
	return ;
}

// least recently used algorithm
void lru ()
{
	// represent pages currently in the frame
	unordered_set <int> inFrame;
	// store pages in FIFO order
	vector <int> index;
	// store pages that are "dirty"
	vector <int> dirty;

	int offset = bitOffset(page_size);

	for (int i = 0; i < addresses.size(); i++)
	{
		int pageNumber = addresses[i] >> offset;
		if (is_read[i] == false)
		{
			dirty.push_back(pageNumber);
		}

		// sort dirty pages and remove any duplicates
		sort(dirty.begin(), dirty.end());
		dirty.erase(unique(dirty.begin(), dirty.end()), dirty.end());

		// if there is still space in the frame
		if (inFrame.size() < frame_num)
		{	
			// insert page into frame if page is not present
			if (inFrame.find(pageNumber) == inFrame.end())
			{
				// insert current page into frame
				inFrame.insert(pageNumber);
				// increment page fault as page was not in the frame
				page_faults++;
				// push current page into the queue
				index.push_back(pageNumber);
				// increment reads when address is written into frame
				reads++;
			}
			else
			{
				int counter = 0;
				// remove dirty page from vector, becomes clean
				for (const int& i : index)
				{
					if (i == pageNumber)
					{
						index.erase(index.begin() + counter);
						break;
					}
					counter++;
				}

				// remove address from frame
				inFrame.erase(pageNumber);
				// insert current page into frame
				inFrame.insert(pageNumber);
				// push current page into queue
				index.push_back(pageNumber);	
			}
		}
		// if the frame is full
		else
		{
			if (inFrame.find(pageNumber) == inFrame.end())
			{
				// store oldest address in the queue to
				// be used to find and erase address from
				// frame
				int addr = index.front();

				int counter = 0;
				// remove dirty page from vector, becomes clean
				for (const int& i : dirty)
				{
					if (i == addr)
					{
						writes++;
						dirty.erase(dirty.begin() + counter);
						break;
					}
					counter++;
				}

				// remove oldest address from the queue
				index.erase(index.begin() + 0);
				// remove address from frame
				inFrame.erase(addr);
				// insert current page into frame
				inFrame.insert(pageNumber);
				// push current page into queue
				index.push_back(pageNumber);
				// increment page_faults and reads when 
				// an address is overwritten
				page_faults++;
				reads++;
			}
			else
			{
				int counter = 0;
				// remove dirty page from vector, becomes clean
				for (const int& i : index)
				{
					if (i == pageNumber)
					{
						index.erase(index.begin() + counter);
						break;
					}
					counter++;
				}

				// remove address from frame
				inFrame.erase(pageNumber);
				// insert current page into frame
				inFrame.insert(pageNumber);
				// push current page into queue
				index.push_back(pageNumber);	
			}
		}
	}
}

// for the lru algorithm, returns true if the pte is in the page table
bool in_table (uint64_t* page_table, uint64_t pte)
{
	for (int i = 0; i < frame_num; ++i)
	{
		if ((pte / page_size) == (page_table[i] / page_size))
		{
			return true;
		}
	}
	return false;
}

// for the lru algorithm, returns the index of the page table where the pte is
int find_pte_in_table (uint64_t* page_table, uint64_t pte)
{
	for (int i = frame_num; i--;)
	{
		if ((pte / page_size) == (page_table [i] / page_size))
			return i;
	}

	// something went wrong, printing debug info
	cerr << "invalid index" << endl << "page table: " << endl;
	for (int i = 0; i < frame_num; ++i)
	{
		cerr << page_table[i] << endl;
	}
	cerr << "pte: " << endl << pte << endl;

	exit (1);
	return -1;
}

// updates the bits depending on the state at the end of each time interval (1 <= interval <= 12)
void arb_update_bits (uint* bits)
{
	for (int i = 0; i < frame_num; ++i)
	{
		bits[i] = bits[i] >> 1;
	}

	return ;
}

// replaces the lowest bit value or fifo if the same
void arb_replace (uint64_t* page_table, bool* is_dirty, uint64_t pte, uint* bits, uint* state, int max, bool debug)
{
	// initialising variables
	int replace_index = -1;
	uint64_t smallest_val = 0xFFFFFFFFFFFFFFFF;

	// duplicates
	uint64_t* temp = new uint64_t[frame_num] ();
	uint* temp_bits = new uint[frame_num] ();
	uint* temp_state = new uint[frame_num] ();
	bool* temp_dirt = new bool[frame_num] ();

	// finding the replacement index with smallest bits, then fifo if same
	for (int i = 0; i < frame_num; ++i)
	{
		if (state[i] == 0 && smallest_val > bits[i])
		{
			smallest_val = bits[i];
			replace_index = i;
		}
	}

	if (replace_index == -1)
	{
		for (int i = 0; i < frame_num; ++i)
		{
			if (smallest_val > bits[i])
			{
				smallest_val = bits[i];
				replace_index = i;
			}
		}
	}

	// duplicateing input arrays
	for (int i = 0; i < frame_num; ++i)
	{
		temp[i] = page_table[i];
		temp_bits[i] = bits[i];
		temp_dirt[i] = is_dirty[i];
		temp_state[i] = state[i];
	}

	// if writing the mem
	if (is_dirty[replace_index] && (temp[replace_index] / page_size != pte / page_size))
	{
		// debug
		if (debug)
			cout << "   REPLACE: page" << setw(4) << (signed) temp[replace_index]/ page_size << " (DIRTY)  " ;
		writes ++;
	}
	else
	{
		// debug
		if (debug)
			cout << "   REPLACE: page" << setw(4) << (signed) temp[replace_index]/ page_size << "          ";
	}
	
	// pushing the stack up and replacing with new page table entry
	for (int i = 0; i < frame_num; ++i)
	{
		// unchanged
		if (i < replace_index)
		{
			continue;
		}
		// slide forward one
		else if (i > replace_index)
		{
			page_table[i-1] = temp[i];
			bits[i-1] = temp_bits[i];
			state[i-1] = temp_state[i];
			is_dirty[i-1] = temp_dirt[i];
		}
	}
	
	// push new pte to last index (bottom of fifo stack)
	page_table [frame_num - 1] = pte;
	bits [frame_num - 1] = max;
	state [frame_num - 1] = 1;
	is_dirty[frame_num - 1] = false;

	free (temp);
	free (temp_bits);
	free (temp_state);
	free (temp_dirt);
	return ;
}

// additional reference bits
void arb (int num_bits, int interval)
{
	bool debug = false;
	// represents the page table
	uint64_t* page_table = new uint64_t[frame_num] ();
	
	for (int i = 0; i < frame_num; ++i)
	{
		page_table[i] = -1;
	}

	bool* is_dirty = new bool [frame_num] () ;

	// checks if change has occured in the interval
	uint* state = new uint[frame_num] () ;
	// records the bits
	uint* bits = new uint[frame_num] () ;
	
	uint max = power (2, num_bits-1);
	int j = 0;
	
	//debug
	if (debug)
		cout << "Details(Just for debug):"<< endl;

	for (int i = 0; i < addresses.size(); i++)
	{
		j++;
		// debug
		if (debug)
			cout << "Time: " << setw(3) << i+1 << "  ";

		// checking for page fault
		if (in_table (page_table, addresses.at(i)) )
		{
			// debug
			if (debug)
			{
				cout << "HIT:     page" << setw(4) << addresses.at (i)/ page_size << "                              " ;
			}

			int index = find_pte_in_table (page_table, addresses.at(i));
			if (state[index] != 1)
			{
				bits[index] += max;
				state[index] =1 ;
			}
			
		}
		else
		{
			// debug
			if (debug)
			{
				cout << "MISS:    page" << setw(4) << addresses.at (i)/ page_size ;
			}

			reads ++;
			page_faults ++;
			arb_replace (page_table, is_dirty, addresses.at(i), bits, state, max, debug);
		}

		// if the operation is READ
		if (is_read[i] == true)
		{
			// does nothing
		}
		// if the operation is WRITE
		else
		{
			int index = find_pte_in_table (page_table, addresses.at(i));
			is_dirty[index] = true;
		}

		// debug
		if (debug)
		{
			cout << "frames:" ;
			for (int l = 0; l < frame_num; l++)
			{
				uint64_t mask = 0xFFFFFFFFFFFFFFFF << log_base_2 (page_size);
				cout << dec << setw(3) << (signed) ((page_table[l]) /page_size) << "(" << (bitset <6> (bits[l])) << ") ";
			}
			cout << endl;
		}
		
		// if new arb interval
		if (j == interval)
		{
			j = 0;
			arb_update_bits (bits);
			
			// refreshing state
			free (state);
			state = new uint[frame_num] () ;
		}
	}
	
	// freeing memory
	free (page_table);
	free (is_dirty);
	free (state);
	free (bits);
	return ;
}

// finds the index of the pte in the working space table
int working_space_index (vector <uint64_t> working_space_table, uint64_t pte)
{
	for (int i = 0; i < working_space_table.size(); ++i)
	{
		if (working_space_table.at(i) / page_size == pte / page_size)
		{
			return i;
		}
	}
	return -1;
}

void working_space_update (vector <uint64_t> working_space_table, vector <uint> counter, uint64_t pte, int working_delta)
{
	int index = working_space_index (working_space_table, pte);
	if (index != -1)
	{
		counter.at(index) ++;
		return ;
	}

	if (working_space_table.size() < working_delta)
	{
		
	}
}

//
void wsarb1_replace (uint64_t* page_table, bool* is_dirty, uint64_t pte, uint* bits, vector <uint64_t> working_space, vector <uint> counter, int delta, uint max)
{
	//
	return ;
}

// working set additional reference bits reverse
void wsarb_1 (int num_bits, int interval, int delta)
{
	// represents the page table
	uint64_t* page_table = new uint64_t[frame_num] ();
	bool* is_dirty = new bool [frame_num] () ;

	// checks if change has occured in the interval
	uint* state = new uint[frame_num] () ;
	// records the bits
	uint* bits = new uint[frame_num] () ;
	
	uint max = power (2, num_bits-1);
	int j = 0;
	
	// working space storage
	vector <uint64_t> working_space_table;
	vector <uint> counter ;

	// dumy values
	for (int i = 0; i < frame_num; ++i)
	{
		page_table[i] = -1;
	}

	for (int i = 0; i < addresses.size(); i++)
	{
		j++;
		// checking for page fault
		if (in_table (page_table, addresses.at(i)) )
		{
			int index = find_pte_in_table (page_table, addresses.at(i));
			if (state[index] != 1)
			{
				bits[index] += max;
				state[index] =1 ;
			}
		}
		else
		{
			reads ++;
			page_faults ++;
			arb_replace (page_table, is_dirty, addresses.at(i), bits, state, max, false);
		}

		// if the operation is READ
		if (is_read[i] == true)
		{
			// does nothing
		}
		// if the operation is WRITE
		else
		{
			int index = find_pte_in_table (page_table, addresses.at(i));
			is_dirty[index] = true;
		}
		
		// if new arb interval
		if (j == interval)
		{
			j = 0;
			arb_update_bits (bits);
			
			// refreshing state
			free (state);
			state = new uint[frame_num] () ;
		}
	}
	
	// freeing memory
	free (page_table);
	free (is_dirty);
	free (state);
	free (bits);
	return ;
}

// working set additional reference bits
void wsarb_2 (int num_bits, int interval, int delta)
{
	arb (num_bits, interval);
	return ;
}

// executes stack trace based on algorithm
void execute_trace (string alg, vector <int> alg_args)
{
	if (alg == "FIFO")
	{
		if (alg_args.size() != 0)
		{
			cerr << "invalid number of arguments" << endl;
			exit(1);
		}

		fifo ();
	}

	else if (alg == "LRU")
	{
		if (alg_args.size() != 0)
		{
			cerr << "invalid number of arguments" << endl;
			exit(1);
		}

		lru();
	}

	else if (alg == "ARB")
	{
		if (alg_args.size() != 2)
		{
			cerr << "invalid number of arguments" << endl;
			exit(1);
		}
		
		arb (alg_args[0], alg_args[1]);
	}

	else if (alg == "WSARB-1")
	{
		if (alg_args.size() != 3)
		{
			cerr << "invalid number of arguments" << endl;
			exit(1);
		}
		
		wsarb_1 (alg_args[0], alg_args[1], alg_args[2]);
	}

	else if (alg == "WSARB-2")
	{
		if (alg_args.size() != 3)
		{
			cerr << "invalid number of arguments" << endl;
			exit(1);
		}
		
		wsarb_2 (alg_args[0], alg_args[1], alg_args[2]);
	}

	else
	{
		cerr << "invalid algorithm" << endl;
		exit(1);
	}
	return;
}

// writes the output
void print_output()
{
	cout << "events in trace: " << dec << events << endl;
	cout << "total disk reads: " << reads << endl;
	cout << "total disk writes: " << writes << endl;
	cout << "page faults: " << page_faults << endl;
	return;
}

// converts the input file into variables
void input_file_parser(char const* argv)
{
	// Create and open a text file
	ifstream input_file(argv);
	string input_txt = "";

	while (getline (input_file, input_txt))
	{
		int i = 2;
		bool comment = false;
		uint64_t temp = 0;

		if (input_txt[0] == '#')
		{
			comment = true;
		}
		else if (input_txt[0] == 'R' || input_txt[0] == 'r')
		{
			is_read.push_back(true);
		}
		else if (input_txt[0] == 'W' || input_txt[0] == 'w')
		{
			is_read.push_back(false);
		}
		else
		{
			cerr << "invalid character" << endl;
			exit(1);
		}

		while (input_txt[i] != '\0' && input_txt[i] != '\n' && input_txt[i] != 13 && !comment)
		{
			if ('0' <= input_txt[i] && input_txt[i] <= '9')
			{
				temp = temp * 16 + input_txt[i] - '0';
			}
			else
			{
				switch (input_txt[i])
				{
					case 'A':
					case 'a': temp = temp * 16 + 10;
					break;

					case 'B':
					case 'b': temp = temp * 16 + 11;
					break;

					case 'C':
					case 'c': temp = temp * 16 + 12;
					break;

					case 'D':
					case 'd': temp = temp * 16 + 13;
					break;

					case 'E':
					case 'e': temp = temp * 16 + 14;
					break;

					case 'F':
					case 'f': temp = temp * 16 + 15;
					break;

					default:
					cerr << "bad digit. Ascii of digit: " << (int) input_txt[i] << endl;
					exit(1);
				}
			}
			++i;
		}
		if (!comment)
		{
			addresses.push_back(temp);
			events++;
		}
	}

	// Close the file
	input_file.close();
	return;
}

// converts the input to variables we can use later
void input_parser(int argc, char const* argv[], string* alg, vector <int>* alg_args)
{
	if (argc < 5)
	{
		cerr << "not enough arguments" << endl;
		exit(1);
	}

	input_file_parser(argv[1]);
	page_size = string_to_int(argv[2]);
	frame_num = string_to_int(argv[3]);
	alg[0] = argv[4];

	for (int i = 5; i < argc; ++i)
	{
		alg_args->push_back(string_to_int(argv[i]));
	}

	return;
}

void debug_print (string alg, vector<int> alg_args)
{
	cout << "alg: " << alg << endl;
	for (int i = 0; i < alg_args.size(); ++i)
	{
		cout << "alg arg " << i << ": " << alg_args[i] << endl;
	}
	cout << "page size: " << page_size << endl;
	cout << "frame num: " << frame_num << endl;
	for (int i = 0; i < addresses.size(); ++i)
	{
		cout << (is_read.at(i) ? "R " : "W ") << hex << setfill('0') << setw(8) << addresses.at(i) << endl;
	}
}

// executes the simulator
int main(int argc, char const* argv[])
{
	//clearing memory
	addresses.clear();
	is_read.clear();

	// allocaiting memory
	vector<int> alg_args;
	string* alg = new string [1] ();

	// converts the input to variables
	input_parser(argc, argv, alg, &alg_args);

	// printing variables. Should look like input file
	// debug_print (alg[0], alg_args);

	// execute the stack trace using input algorithm and input text file
	execute_trace(alg[0], alg_args);
	alg_args.clear();

	// printing output
	print_output();
}