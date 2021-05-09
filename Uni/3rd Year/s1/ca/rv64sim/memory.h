#ifndef MEMORY_H
#define MEMORY_H

/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2020

   Class for memory

**************************************************************** */

#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <cstdio>

class memory {

private:
	// already here
	bool verbose;

  	// a generic chunk of memory stored in the heap
	struct chunk
	{
		uint64_t mapping_index; // for the vector
		uint64_t memory_index;	// for the division
		uint64_t* data = new uint64_t[1024];
	};
	
	// local chunk of memory on the stack for faster operations
	struct cache
	{
		uint64_t mapping_index;
		uint64_t memory_index;
		uint64_t data[1024];
	};

	// main memory separated into chunks of 1024 addresses to regesters and their data
	std::vector<chunk> main;
	// current cache in memory (to reduce the amount of times we go into the heap)
	cache current_cache;

public:

	// Constructor
	memory(bool verbose);

  	// Read a doubleword of data from a doubleword-aligned address.
  	// If the address is not a multiple of 8, it is rounded down to a multiple of 8.
	uint64_t read_doubleword (uint64_t address);

  	// Write a doubleword of data to a doubleword-aligned address.
  	// If the address is not a multiple of 8, it is rounded down to a multiple of 8.
  	// The mask contains 1s for bytes to be updated and 0s for bytes that are to be unchanged.
	void write_doubleword (uint64_t address, uint64_t data, uint64_t mask);

  	// Load a hex image file and provide the start address for execution from the file in start_address.
  	// Return true if the file was read without error, or false otherwise.
	bool load_file(std::string file_name, uint64_t &start_address);

  	// Destructor
	~memory();
};

#endif
