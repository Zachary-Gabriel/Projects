#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "memory.h"
#include "processor.h"
#include "commands.h"

void parse_instruction (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose);

#endif