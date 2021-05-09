#include "storage.h"

using namespace std;

// converts hex into string retaining hex structure
string hex_to_string (uint64_t n)
{
	stringstream s;
	s << hex << n ; 
	return s.str();
}

// creating memory
storage::storage ()
{
	// adding the first chunk of memory to main memory
	chunk initial;
	initial.index = 0;
	main.push_back(initial);

	// modifying cache value
	current_cache.index = 0;
}

// writing to memory
void storage::write (uint64_t address, uint64_t data, uint64_t mask)
{
	address -= address % 8; // alligning address
	uint64_t condensed_index = address / 8; // the address gaps should be 8 (double word difference), but the array gaps should be 1 (index difference)
	uint64_t new_index = condensed_index / 1024; // integer division to find the multiple of 1024 hence which cache the address points to
	uint64_t cache_index = condensed_index % 1024; // remainder for the loaction inside the cache

	// if address is not in current cache
	if (current_cache.index != new_index)
	{
		uint64_t length = main.size();
		bool make_new = true;
		uint64_t old_index = current_cache.index;

		// checking to see if chunk is already in memory
		for (uint64_t i = 0; i < length; ++i)
		{
			if (main.at(i).index == new_index)
			{
				make_new = false;
			}
		}

		// make a new chunk if not in memory
		if (make_new)
		{
			chunk new_chunk;
			new_chunk.index = new_index;
			main.push_back(new_chunk);
		}


		for(unsigned int i = 0; i < 1024; ++i)
		{
			main.at(old_index).data[i] = current_cache.data[i]; // storing local cache into memory
			current_cache.data[i] = main.at(new_index).data[i]; // storing chunk into cache
		}
	}
	
	// applying the mask using the steps in the guideline
	uint64_t step_1 = data & mask;
	uint64_t step_2 = ~mask & current_cache.data[cache_index];
	uint64_t result = step_1 | step_2 ;

	current_cache.data[cache_index] = result;
	return ;
}

// reading from memory
uint64_t storage::read (uint64_t address)
{
	address -= address % 8; // alligning address
	uint64_t condensed_index = address / 8; // the address gaps should be 8 (double word difference), but the array gaps should be 1 (index difference)
	uint64_t new_index = condensed_index / 1024; // integer division to find the multiple of 1024 hence which cache the address points to
	uint64_t cache_index = condensed_index % 1024; // remainder for the loaction inside the cache

	// if address is not in current cache
	if (current_cache.index != new_index)
	{
		uint64_t length = main.size();
		bool make_new = true;
		uint64_t old_index = current_cache.index;

		// checking to see if chunk is already in memory
		for (uint64_t i = 0; i < length; ++i)
		{
			if (main.at(i).index == new_index)
			{
				make_new = false;
			}
		}

		// make a new chunk if not in memory
		if (make_new)
		{
			chunk new_chunk;
			new_chunk.index = new_index;
			main.push_back(new_chunk);
		} 
		
		// copy the old chunk over otherwise
		else 
		{ 
			for(unsigned int i = 0; i < 1024; ++i)
			{
				main.at(old_index).data[i] = current_cache.data[i]; // storing local cache into memory
				current_cache.data[i] = main.at(new_index).data[i]; // storing chunk into cache
			}
		}
	}
	// returning the data from the register	
	return current_cache.data[cache_index];
}

// delete memory
storage::~storage ()
{
	uint64_t length = main.size();
	for (uint64_t i = 0; i < length; ++i)
	{
		delete[] main.at(i).data;
	}
	main.clear();
}