#ifndef PROCESSOR_H
#define PROCESSOR_H

/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2020

   Class for processor

**************************************************************** */

#include "memory.h"
 
using namespace std;

class processor {

private:

	bool verbose, breakpoint_check = false, halt = false, increment = true, is_except = false;
	int exception = -1;
	memory* main_memory;
	bool stage2;
	uint64_t pc, b, instructions;
	uint64_t xn[32];
	int prv;
	uint64_t mvendorid;
	uint64_t marchid;
	uint64_t mimpid;
	uint64_t mhartid;
	uint64_t mstatus;
	uint64_t misa;
	uint64_t mie;
	uint64_t mtvec;
	uint64_t mscratch;
	uint64_t mepc;
	uint64_t mcause;
	uint64_t mtval;
	uint64_t mip;
public:

  	// Consructor
	processor(memory* main_memory, bool verbose, bool stage2);

  	// Display PC value
	void show_pc();

  	// Set PC to new value
	void set_pc(uint64_t new_pc);

  	// Display register value
	void show_reg(unsigned int reg_num);

  	// Set register to new value
	void set_reg(unsigned int reg_num, uint64_t new_value);

  	// Execute a number of instructions
	void execute(unsigned int num, bool);

  	// Clear breakpoint
	void clear_breakpoint();

  	// Set breakpoint at an address
	void set_breakpoint(uint64_t address);

  	// Show privilege level
  	// Empty implementation for stage 1, required for stage 2
	void show_prv();

  	// Set privilege level
  	// Empty implementation for stage 1, required for stage 2
	void set_prv(unsigned int prv_num);
	unsigned int get_prv();

  	// Display CSR value
  	// Empty implementation for stage 1, required for stage 2
	void show_csr(unsigned int csr_num);

  	// Set CSR to new value
  	// Empty implementation for stage 1, required for stage 2
	void set_csr(unsigned int csr_num, uint64_t new_value);
	uint64_t get_csr(unsigned int csr_num);

  	// Returns the number of intructions executed since the start of simulation.
	uint64_t get_instruction_count();

  	// Used for Postgraduate assignment. Undergraduate assignment can return 0.
	uint64_t get_cycle_count();

  	// Getting the program counter for computations
	uint64_t get_pc (void);

  	// Getting the register value of the input index
	uint64_t get_reg (unsigned int n);

	void set_inc (bool inc);
	
	bool get_inc (void);

	void set_is_except (bool);

	bool get_is_except(void);
};

#endif
