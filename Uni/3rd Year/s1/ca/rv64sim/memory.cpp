/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2020

   Class members for memory

**************************************************************** */

#include "memory.h"
using namespace std;

// Constructor
memory::memory(bool verbose) 
{
	this->verbose = verbose;
	// adding the first chunk of memory to main memory
	chunk initial_chunk;
	initial_chunk.mapping_index = 0;
	initial_chunk.memory_index = 0;
	main.push_back (initial_chunk);

	// modifying cache value
	current_cache.mapping_index = 0;
	current_cache.memory_index = 0;
}

// Read a doubleword of data from a doubleword-aligned address.
// If the address is not a multiple of 8, it is rounded down to a multiple of 8.
uint64_t memory::read_doubleword (uint64_t address) 
{
	if (verbose)
	{
		cout << "memory::read_doubleword address: " << hex << address;
	}
	address -= address % 8; // alligning address
	uint64_t memory_index = address / 8; // the address gaps should be 8 (double word difference), but the array gaps should be 1 (index difference)
	uint64_t condensed_memory_index = memory_index / 1024; // integer division to find the memory chunk using its memory_index (ideally the same as the cache's)
	uint64_t cache_index = memory_index % 1024; // remainder for the loaction inside the cache
	// for finding the index of the vector
	uint64_t mapping_index = 0;
	// if address is not in current cache
	if (current_cache.memory_index != condensed_memory_index)
	{
		bool exists = false;
		// checking to see if chunk is already in memory
		for (unsigned int i = 0; i < main.size(); ++i)
		{
			if (main.at(i).memory_index == condensed_memory_index)
			{
				mapping_index = i;
				exists = true;
			}
		}

		// make a new chunk if not in memory and set the cache to zeros
		if (!exists)
		{
			// making chunk
			chunk temp_chunk;
			temp_chunk.mapping_index = main.size();
			temp_chunk.memory_index = condensed_memory_index;
			main.push_back (temp_chunk);

			// saving cache to main and setting new array value to zero (simulate new chunk of memory allocation)
			uint64_t temp_mapping_index = current_cache.mapping_index;
			for (unsigned int i = 0; i < 1024; ++i)
			{
				main.at(temp_mapping_index).data[i] = current_cache.data[i];
				current_cache.data[i] = 0;
			}
			current_cache.mapping_index = temp_chunk.mapping_index;
			current_cache.memory_index = condensed_memory_index;

		}
		// copy the old chunk over otherwise
		else
		{
			uint64_t temp_mapping_index = current_cache.mapping_index;
			for (unsigned int i = 0; i < 1024; ++i)
			{
				main.at(temp_mapping_index).data[i] = current_cache.data[i];
				current_cache.data[i] = main.at(mapping_index).data[i];
			}
			current_cache.mapping_index = mapping_index;
			current_cache.memory_index = condensed_memory_index;
		}
	}
	if (verbose)
	{
		cout << ", data: " << current_cache.data[cache_index] << endl;
	}
	// returning the data from the register
	return current_cache.data[cache_index];
}

// Write a doubleword of data to a doubleword-aligned address.
// If the address is not a multiple of 8, it is rounded down to a multiple of 8.
// The mask contains 1s for bytes to be updated and 0s for bytes that are to be unchanged.
void memory::write_doubleword (uint64_t address, uint64_t data, uint64_t mask) 
{
	if (verbose)
	{
		cout << "memory::write_doubleword address: " << hex << address << ", data " << data << ", mask: " << mask << endl;
	}

	address -= address % 8; // alligning address
	uint64_t memory_index = address / 8; // the address gaps should be 8 (double word difference), but the array gaps should be 1 (index difference)
	uint64_t condensed_memory_index = memory_index / 1024; // integer division to find the memory chunk using its memory_index (ideally the same as the cache's)
	uint64_t cache_index = memory_index % 1024; // remainder for the loaction inside the cache
	// for finding the index of the vector
	uint64_t mapping_index = 0;
	// if address is not in current cache
	if (current_cache.memory_index != condensed_memory_index)
	{
		bool exists = false;
		// checking to see if chunk is already in memory
		for (unsigned int i = 0; i < main.size(); ++i)
		{
			if (main.at(i).memory_index == condensed_memory_index)
			{
				mapping_index = i;
				exists = true;
			}
		}

		// make a new chunk if not in memory and set the cache to zeros
		if (!exists)
		{
			// making chunk
			chunk temp_chunk;
			temp_chunk.mapping_index = main.size();
			temp_chunk.memory_index = condensed_memory_index;
			main.push_back (temp_chunk);

			// saving cache to main and setting new array value to zero (simulate new chunk of memory allocation)
			uint64_t temp_mapping_index = current_cache.mapping_index;
			for (unsigned int i = 0; i < 1024; ++i)
			{
				main.at(temp_mapping_index).data[i] = current_cache.data[i];
				current_cache.data[i] = 0;
			}
			current_cache.mapping_index = temp_chunk.mapping_index;
			current_cache.memory_index = condensed_memory_index;
		}
		// copy the old chunk over otherwise
		else
		{
			uint64_t temp_mapping_index = current_cache.mapping_index;
			for (unsigned int i = 0; i < 1024; ++i)
			{
				main.at(temp_mapping_index).data[i] = current_cache.data[i];
				current_cache.data[i] = main.at(mapping_index).data[i];
			}
			current_cache.mapping_index = mapping_index;
			current_cache.memory_index = condensed_memory_index;
		}
	}
	// applying the mask using the steps in the guideline
	uint64_t step_1 = data & mask;
	uint64_t step_2 = ~mask & current_cache.data[cache_index];
	// storing in register using or bitwise OR
	current_cache.data[cache_index] = step_1 | step_2 ;
	return ;
}

// Load a hex image file and provide the start address for execution from the file in start_address.
// Return true if the file was read without error, or false otherwise.
bool memory::load_file(string file_name, uint64_t &start_address) 
{
	ifstream input_file(file_name);
	string input;
	unsigned int line_count = 0;
	unsigned int byte_count = 0;
	char record_start;
	char byte_string[3];
	char halfword_string[5];
	unsigned int record_length;
	unsigned int record_address;
	unsigned int record_type;
	unsigned int record_data;
	unsigned int record_checksum;
	bool end_of_file_record = false;
	uint64_t load_address;
	uint64_t load_data;
	uint64_t load_mask;
	uint64_t load_base_address = 0x0000000000000000ULL;
	start_address = 0x0000000000000000ULL;
	if (input_file.is_open()) 
	{
		while (true) 
		{
			line_count++;
			input_file >> record_start;
			if (record_start != ':') 
			{
				cout << "Input line " << dec << line_count << " does not start with colon character" << endl;
				return false;
			}
			input_file.get(byte_string, 3);
			sscanf(byte_string, "%x", &record_length);
			input_file.get(halfword_string, 5);
			sscanf(halfword_string, "%x", &record_address);
			input_file.get(byte_string, 3);
			sscanf(byte_string, "%x", &record_type);
			switch (record_type) 
			{
      case 0x00:  // Data record
      for (unsigned int i = 0; i < record_length; i++) 
      {
      	input_file.get(byte_string, 3);
      	sscanf(byte_string, "%x", &record_data);
      	load_address = (load_base_address | (uint64_t)(record_address)) + i;
      	load_data = (uint64_t)(record_data) << ((load_address % 8) * 8);
      	load_mask = 0x00000000000000ffULL << ((load_address % 8) * 8);
      	write_doubleword(load_address & 0xfffffffffffffff8ULL, load_data, load_mask);
      	byte_count++;
      }
      break;
      case 0x01:  // End of file
      end_of_file_record = true;
      break;
      case 0x02:  // Extended segment address (set bits 19:4 of load base address)
      load_base_address = 0x0000000000000000ULL;
      for (unsigned int i = 0; i < record_length; i++) 
      {
      	input_file.get(byte_string, 3);
      	sscanf(byte_string, "%x", &record_data);
      	load_base_address = (load_base_address << 8) | (record_data << 4);
      }
      break;
      case 0x03:  // Start segment address (ignored)
      for (unsigned int i = 0; i < record_length; i++) 
      {
      	input_file.get(byte_string, 3);
      	sscanf(byte_string, "%x", &record_data);
      }
      break;
      case 0x04:  // Extended linear address (set upper halfword of load base address)
      load_base_address = 0x0000000000000000ULL;
      for (unsigned int i = 0; i < record_length; i++) 
      {
      	input_file.get(byte_string, 3);
      	sscanf(byte_string, "%x", &record_data);
      	load_base_address = (load_base_address << 8) | (record_data << 16);
      }
      break;
      case 0x05:  // Start linear address (set execution start address)
      start_address = 0x0000000000000000ULL;
      for (unsigned int i = 0; i < record_length; i++) 
      {
      	input_file.get(byte_string, 3);
      	sscanf(byte_string, "%x", &record_data);
      	start_address = (start_address << 8) | record_data;
      }
      break;
  }
  input_file.get(byte_string, 3);
  sscanf(byte_string, "%x", &record_checksum);
  input_file.ignore();
  if (end_of_file_record)
  	break;
}
input_file.close();
cout << dec << byte_count << " bytes loaded, start address = "
<< setw(16) << setfill('0') << hex << start_address << endl;
return true;
}
else 
{
	cout << "Failed to open file" << endl;
	return false;
}
}

// Destructor
memory::~memory() 
{
	uint64_t length = main.size();
	for (uint64_t i = 0; i < length; ++i)
	{
		delete[] main.at(i).data;
	}
	main.clear();
}