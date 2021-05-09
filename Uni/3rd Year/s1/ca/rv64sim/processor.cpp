#include "memory.h"
#include "processor.h"
#include "commands.h"
#include "instructions.h"

using namespace std;

// Consructor
processor::processor(memory* main_memory, bool verbose, bool stage2)
{
    this->main_memory = main_memory;
    this->verbose = verbose;
    this->stage2 = stage2;

    // stage 2
    this->prv = 3; // 3 for machine, 0 for user
    
    // static registers for this task 
    //0xF11->F14
    this->mvendorid = 0;
    this->marchid = 0;
    this->mimpid = 0x2020020000000000;
    this->mhartid = 0;
    
    // dynamic registers
    // 0x300 -> 301
    this->mstatus = 0x200000000; // mie = 3 (machine interupt enabled), mpie = 7 (machine previous interupt enabled), mpp[1:0] = 12, 11 (machine previous prv)
    this->misa = 0x8000000000100100; // can be written to but stays the same
    
    // 0x304 -> 305
    this->mie = 0; // machine interupt enabled: usie, msie, utie, mtie, ueie, meie
    this->mtvec = 0; // address of error handling

    //0x340 -> 0x344
    this->mscratch= 0; // dunno
    this->mepc= 0; // sets the value of pc after error. 0th is always 0. 1st is 0 for 32 bit allignment
    this->mcause= 0; // ebreak error handler. first bit 0 for cpu errors. first bit one for interupts.
    this->mtval= 0; // value used for error handling (dodgey/ missalligned addresses)
    this->mip= 0; // machine interupt pending

    //setting all the registers to 0
    for (int i = 0; i < 32; ++i)
    {
        xn[i] = 0;
    }
}

// Display PC value
void processor::show_pc()
{
    if (verbose)
    {
        cout << "getting pc" << endl;
    }
    cout << setw(16) << setfill('0') << hex << pc << endl;
    return ;
}

// Set PC to new value
void processor::set_pc(uint64_t new_pc)
{
    if (verbose)
    {
        cout << hex << "pc set to " << new_pc << endl;
    }
    pc = new_pc;
    return ;
}

uint64_t processor::get_pc (void)
{
    return pc;
}

uint64_t processor::get_reg (unsigned int n)
{
    if (n == 0)
    {
        return 0x0000000000000000 ;
    }
    return xn[n];
}

// Display register value
void processor::show_reg(unsigned int reg_num)
{
    if (verbose)
    {
        cout << dec << "getting register: xn[" << reg_num << "]" << endl;
    }
    
    if (reg_num == 0)
    {
        cout << "0000000000000000" << endl;
        return ;
    }

    cout << setw(16) << setfill('0') << hex << xn[reg_num] << endl;
    return ;
}

// Set register to new value
void processor::set_reg(unsigned int reg_num, uint64_t new_value)
{
    if (reg_num != 0)
        xn[reg_num] = new_value;
    return ;
}
void processor::set_is_except (bool val)
{
    is_except = val;
}

bool processor::get_is_except ()
{
    return is_except;
}

void processor::set_inc (bool inc)
{
    this-> increment = inc;
    return ;
}

bool processor::get_inc (void)
{
    return increment;
}

// Execute a number of instructions
void processor::execute(unsigned int num, bool breakpoint_check)
{
    for (unsigned int i = 0; i < num; ++i)
    {
        if (this->breakpoint_check == true && breakpoint_check == true)
        {
            if (b == pc)
            {
                cout << hex << "Breakpoint reached at ";
                cout << setw(16) << pc << endl;
                // parse_instruction (0x100073, this->main_memory, this, verbose); // ebreak instruction
                return ;
            }
        }

        // do instruction
        uint64_t instruction =  main_memory->read_doubleword(pc) ;
        uint64_t first_word_mask = 0x00000000FFFFFFFF;

        // first word
        if (get_pc() % 8 == 0)
        {
            instruction = first_word_mask & instruction;
        }
        // second word
        else if (get_pc() % 8 == 4)
        {
            instruction = ~first_word_mask & instruction;
            instruction = instruction >> 32 ;
        }
        // misaligned pc
        else 
        {
            set_csr (0x343, get_pc()); // saving dodgey address
            set_csr (0x300, 0x0000000200001800); // mpp 3
            set_csr (0x341, get_pc() - (get_pc() % 4)); // realligning pc by rounding down for next instruction
            set_csr (0x342, 0); // 0 Instruction address misaligned
            set_pc (get_csr(0x305)); // moving pc to error handler

            return ;
        }

        if (!halt)
        {
            parse_instruction (instruction, this->main_memory, this, verbose);
            if (increment)
                pc += 4;
            increment = true;

            if (!is_except)
                ++instructions;
            is_except = false;
        }
    }
    return ;
}

// Clear breakpoint
void processor::clear_breakpoint()
{
    breakpoint_check = false;
    b = 0;
    return ;
}

// Set breakpoint at an address
void processor::set_breakpoint(uint64_t address)
{
    if (verbose)
    {
        cout << "breakpoint set at " << hex << address << endl;
    }

    this->breakpoint_check = true;
    b = address;
    return ;
}

// Show privilege level
// Empty implementation for stage 1, required for stage 2
void processor::show_prv()
{
    if(prv == 3)
        cout << "3 (machine)" << endl;
    if (prv == 0)
        cout << "0 (user)" << endl;
    return ;
}

// Set privilege level
// Empty implementation for stage 1, required for stage 2
void processor::set_prv(unsigned int prv_num)
{
    prv = prv_num;
    return ;
}

unsigned int processor::get_prv()
{
    return prv;
}

// Display CSR value
// Empty implementation for stage 1, required for stage 2
void processor::show_csr(unsigned int csr_num)
{
    if (verbose)
        cout << "getting csr: " << hex << csr_num << endl;

    switch (csr_num)
    {
        case 0xF11:
        cout << setw(16) << setfill('0') << "0000000000000000" << endl;
        break;

        case 0xF12:
        cout << setw(16) << setfill('0') << "0000000000000000" << endl;
        break;

        case 0xF13:
        cout << setw(16) << setfill('0') << "2020020000000000" << endl;
        break;

        case 0xF14:
        cout << setw(16) << setfill('0') << "0000000000000000" << endl;
        break;
        
        case 0x300:
        cout << setw(16) << setfill('0') << hex << mstatus << endl;
        break;

        case 0x301:
        cout << setw(16) << setfill('0') << hex << misa << endl;
        break;

        case 0x304:
        cout << setw(16) << setfill('0') << hex << mie << endl;
        break;

        case 0x305:
        cout << setw(16) << setfill('0') << hex << mtvec << endl;
        break;

        case 0x340:
        cout << setw(16) << setfill('0') << hex << mscratch << endl;
        break;

        case 0x341:
        cout << setw(16) << setfill('0') << hex << mepc << endl;
        break;        

        case 0x342:
        cout << setw(16) << setfill('0') << hex << mcause << endl;
        break;

        case 0x343:
        cout << setw(16) << setfill('0') << hex << mtval << endl;
        break;

        case 0x344:
        cout << setw(16) << setfill('0') << hex << mip << endl;
        break;
    }
    
    return ;
}

// Set CSR to new value
// Empty implementation for stage 1, required for stage 2
void processor::set_csr(unsigned int csr_num, uint64_t new_value)
{
    if (verbose)
    {
        cout << "setting csr[" << hex << csr_num << "] to " << new_value << endl;
    }
    switch (csr_num)
    {   
        case 0xF11:
        case 0xF12:
        case 0xF13:
        case 0xF14:
        cout << "Illegal write to read-only CSR" << endl;
        return;

        case 0x300:
        mstatus = (new_value & 0x0000000200001888) | 0x200000000;
        return;

        case 0x301:
        // misa remains fixed
        return;

        case 0x304:
        mie = new_value & 0x0000000000000999;
        return;

        case 0x305: 
        
        if ((new_value & 1) == 0) // All exceptions set pc to BASE fist bit = 0.
        {
            mtvec = new_value & 0xFFFFFFFFFFFFFFFC;
        }

        else if ((new_value & 1) == 1) // vectored = BASE+4×cause first bit = 1
        {
            // popping the interupt bit off the exception
            // uint64_t cause = mcause << 1;
            // cause = mcause >> 1;
            // mtvec = new_value + 4*cause;

            // bits 2 to 7 are fixed at 0
            mtvec = new_value & 0xFFFFFFFFFFFFFF01;
        }
        else // for BASE 2 and 3
        {
            mtvec = 0;
        }

        return;

        case 0x340:
        mscratch = new_value;
        return;

        case 0x341:
        mepc = new_value & 0xFFFFFFFFFFFFFFFC;
        return;        

        case 0x342:
        mcause = new_value & 0x800000000000000F;
        return;

        case 0x343:
        mtval = new_value;
        return;

        case 0x344:
        mip = new_value & 0x0000000000000999;
        return;
        
    }
    cout << "Illegal CSR number" << endl;
    return ;
}

uint64_t processor::get_csr(unsigned int csr_num)
{
    switch (csr_num)
    {
        case 0xF11:
        return mvendorid;

        case 0xF12:
        return marchid;

        case 0xF13:
        return mimpid;

        case 0xF14:
        return mhartid;
        
        case 0x300:
        return mstatus;

        case 0x301:
        return misa;

        case 0x304:
        return mie;

        case 0x305:
        return mtvec;

        case 0x340:
        return mscratch;

        case 0x341:
        return mepc;

        case 0x342:
        return mcause;

        case 0x343:
        return mtval;

        case 0x344:
        return mip;
    }
    return 0;
}

// returns the number of intructions executed since the start of simulation.
uint64_t processor::get_instruction_count()
{
    if (verbose)
    {
        cout << "getting instructions" << endl;
    }
    return instructions;
}

// Used for Postgraduate assignment. Undergraduate assignment can return 0.
uint64_t processor::get_cycle_count() 
{
    return 0;
}