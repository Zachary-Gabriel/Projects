#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <sstream>

// converts hex into string retaining hex structure
std::string hex_to_string (uint64_t n);

class storage
{
private:
	// a generic chunk of memory stored in the heap
	struct chunk
	{
		uint64_t index;
		uint64_t* data = new uint64_t[1024];
	};
	
	// local chunk of memory on the stack for faster operations
	struct cache
	{
		uint64_t index;
		uint64_t data[1024];
	};

	// main memory separated into chunks of 1024 addresses to regesters and their data
	std::vector<chunk> main;
	// current cache in memory (to reduce the amount of times we go into the heap)
	cache current_cache;
public:
	// creating memory object
	storage ();
	
	// writing to memory
	void write (uint64_t address, uint64_t data, uint64_t mask);
	
	// reading from memory
	uint64_t read (uint64_t address);
	
	// delete memory object
	~storage ();
};

#endif