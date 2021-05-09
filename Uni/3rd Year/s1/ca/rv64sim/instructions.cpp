#include "memory.h"
#include "processor.h"
#include "commands.h"
#include "instructions.h"

/* getters of blocks inside instructions */

// returns the opcode of the input instruction
uint64_t get_opcode (uint64_t instruction)
{
    uint64_t opcode_mask = 0x7F; // 0000000 00000 00000 000 00000 1111111
    return opcode_mask & instruction;
}

// returns the fn3 value of the input instruction
uint64_t get_fn3 (uint64_t instruction)
{
    uint64_t fn3_mask = 0x7000; // 0000000 00000 00000 111 00000 0000000
    uint64_t fn3 = fn3_mask & instruction;
    fn3 = fn3 >> 12;
    return fn3;
}

// returns the fn7 value of the input instruction
uint64_t get_fn7 (uint64_t instruction)
{
    uint64_t fn7_mask = 0xFE000000; // 1111111 00000 00000 000 00000 0000000
    uint64_t fn7 = fn7_mask & instruction;
    fn7 = fn7 >> 25;
    return fn7;
}

// returns the index of the return register
uint64_t get_rd (uint64_t instruction)
{
    uint64_t rd_mask = 0xF80; // 0000000 00000 00000 000 11111 0000000
    uint64_t rd = rd_mask & instruction;
    rd = rd >> 7;
    return rd;
}

// returns the index of the first register
uint64_t get_rs1 (uint64_t instruction)
{
    uint64_t rs1_mask = 0xF8000; // 0000000 00000 11111 000 00000 0000000
    uint64_t rs1 = rs1_mask & instruction;
    rs1 = rs1 >> 15;
    return rs1;
}

// returns the index of the second register
uint64_t get_rs2 (uint64_t instruction)
{
    uint64_t rs2_mask = 0x1F00000; // 0000000 11111 00000 000 00000 0000000
    uint64_t rs2 = rs2_mask & instruction;
    rs2 = rs2 >> 20;
    return rs2;
}

// returns the 12 bit immediate between bits 32->20 in the instruction
uint64_t get_12_bit_imm (uint64_t instruction)
{
    uint64_t imm_mask = 0xFFF00000; // 111111111111 00000 000 00000 0000000
    uint64_t imm = imm_mask & instruction;
    imm = imm >> 20;
    return imm ;
}

// assembling the immediate for a branch instruction
uint64_t get_branch_imm (uint64_t instruction)
{
    // immediate 4 -> 1
    uint64_t imm_low_mask = 0xF00; // 0000000 00000 00000 000 11110 0000000
    uint64_t imm_low = imm_low_mask & instruction;
    imm_low = imm_low >> 8;
    imm_low = imm_low << 1;

    // immediate 10 -> 5
    uint64_t imm_high_mask = 0x7E000000; // 0111111 00000 00000 000 00000 0000000
    uint64_t imm_high = imm_high_mask & instruction;
    imm_high = imm_high >> 25;
    imm_high = imm_high << 5;

    // immediate 11
    uint64_t imm_11_mask = 0x80; // 0000000 00000 00000 000 00001 0000000
    uint64_t imm_11 = imm_11_mask & instruction;
    imm_11 = imm_11 >> 7;
    imm_11 = imm_11 << 11;

    // immediate 12
    uint64_t imm_12_mask = 0x80000000; // 1000000 00000 00000 000 00000 0000000
    uint64_t imm_12 = imm_12_mask & instruction;
    imm_12 = imm_12 >> 31;
    imm_12 = imm_12 << 12;

    // combining imm
    return imm_low + imm_high + imm_11 + imm_12;
}

// assembing the immediate for the sstore instructions
uint64_t get_store_imm (uint64_t instruction)
{
    // immediate 4 -> 0
    uint64_t imm_low = get_rd (instruction);

    // immediate 10 -> 5
    uint64_t imm_high_mask = 0xFE000000; // 111111 00000 00000 000 00000 0000000
    uint64_t imm_high = imm_high_mask & instruction;
    imm_high = imm_high >> 25;
    imm_high = imm_high << 5;

    // combining imm
    return imm_low + imm_high;
}

/* parsing individual instructions */

// load byte: rd = M[rs1+imm]
void parse_lb (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiii iiii iiii sssss xxx rrrrr 0000011
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t byte_mask = 0x00000000000000FF;

    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }
    
    if (verbose)
    {
        cout << "lb rd: " << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }
    uint64_t address = cpu-> get_reg (rs1) + imm;
    uint64_t data = main_memory-> read_doubleword (address);

    // first byte
    if (address % 8 == 0)
    {
        data = byte_mask & data;
    }
    // second byte
    else if (address % 8 == 1)
    {
        byte_mask = byte_mask << 8;
        data = byte_mask & data;
        data = data >> 8;
    }
    // third byte
    else if (address % 8 == 2)
    {
        byte_mask = byte_mask << 16;
        data = byte_mask & data;
        data = data >> 16;
    }
    // fourth byte
    else if (address % 8 == 3)
    {
        byte_mask = byte_mask << 24;
        data = byte_mask & data;
        data = data >> 24;
    }
    // fifth byte
    else if (address % 8 == 4)
    {
        byte_mask = byte_mask << 32;
        data = byte_mask & data;
        data = data >> 32;
    }
    // sixth byte
    else if (address % 8 == 5)
    {
        byte_mask = byte_mask << 40;
        data = byte_mask & data;
        data = data >> 40;
    }
    // seventh byte
    else if (address % 8 == 6)
    {
        byte_mask = byte_mask << 48;
        data = byte_mask & data;
        data = data >> 48;
    }
    // eight byte
    else
    {
        byte_mask = byte_mask << 56;
        data = byte_mask & data;
        data = data >> 56;
    }

    // if negative
    if (data >> 7 == 1)
    {
        data = 0xFFFFFFFFFFFFFF00 | data;
    }
    cpu-> set_reg (rd, data);
    return;
}

// load half word
void parse_lh (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiii iiii iiii sssss xxx rrrrr 0000011
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t half_word_mask = 0x000000000000FFFF;
    uint64_t data;
    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    uint64_t address = cpu-> get_reg (rs1) + imm ;
    data = main_memory-> read_doubleword (address);

    if (address % 2 == 0)
    {
        // first half word
        if (address % 8 == 0)
        {
            data = half_word_mask & data;
        }
        // second half word
        else if (address % 8 == 2)
        {
            half_word_mask = half_word_mask << 16;
            data = half_word_mask & data;
            data = data >> 16;
        }
        // third half word
        else if (address % 8 == 4)
        {
            half_word_mask = half_word_mask << 32;
            data = half_word_mask & data;
            data = data >> 32;
        }
        // fourth half word
        else
        {
            half_word_mask = half_word_mask << 48;
            data = half_word_mask & data;
            data = data >> 48;
        }

        // if negative
        if (data >> 15 == 1)
        {
            data = 0xFFFFFFFFFFFF0000 | data;
        }

        cpu-> set_reg (rd, data);

        if (verbose)
        {
            cout << "lh rd: " << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
        }
    }
    else
    {
        /* throw error */
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 4); // mcause load address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }
}

// load word: rd = M[rs1+imm]
void parse_lw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiii iiii iiii sssss 011 rrrrr 0000011
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t word_mask = 0xFFFFFFFFFFFFFFFF;

    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "lw rd: " << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    uint64_t address = cpu-> get_reg (rs1) + imm ;

    if (address % 4 == 0)
    {
        uint64_t data = main_memory-> read_doubleword (address);

        // first word
        if (address % 8 == 0)
        {
            word_mask = word_mask >> 32;
            data = word_mask & data;
        }
        // second word
        else
        {
            word_mask = word_mask << 32;
            data = word_mask & data;
            data = data >> 32;
        }

        // if negative
        if (data >> 31 == 1)
        {
            data = 0xFFFFFFFF00000000 | data;
        }

        cpu-> set_reg (rd, data);
    }
    else
    {
        /* throw error */
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 4); // mcause load address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }
    return ;
}

// load double word rd = M[rs1+imm]
void parse_ld (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiii iiii iiii sssss 011 rrrrr 0000011
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }
    
    if (verbose)
    {
        cout << "ld rd: " << dec << rd << ", rs1: " << rs1 << ", imm: " << hex << imm << endl;
    }

    uint64_t address = cpu-> get_reg (rs1) + imm;

    // misalligned address
    if (address % 8 != 0)
    {
        /* throw error */
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 4); // mcause load address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }
    else
    {
        uint64_t double_word = main_memory-> read_doubleword (address);
        cpu-> set_reg (rd, double_word);
    }

    return ;
}

// load byte unsigned
void parse_lbu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 xxx rd 0000011 
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t byte_mask = 0x00000000000000FF;

    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }
    
    if (verbose)
    {
        cout << "lbu rd: " << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    uint64_t address = cpu-> get_reg (rs1) + imm;
    uint64_t data = main_memory-> read_doubleword (address);

    // first byte
    if (address % 8 == 0)
    {
        data = byte_mask & data;
    }
    // second byte
    else if (address % 8 == 1)
    {
        byte_mask = byte_mask << 8;
        data = byte_mask & data;
        data = data >> 8;
    }
    // third byte
    else if (address % 8 == 2)
    {
        byte_mask = byte_mask << 16;
        data = byte_mask & data;
        data = data >> 16;
    }
    // fourth byte
    else if (address % 8 == 3)
    {
        byte_mask = byte_mask << 24;
        data = byte_mask & data;
        data = data >> 24;
    }
    // fifth byte
    else if (address % 8 == 4)
    {
        byte_mask = byte_mask << 32;
        data = byte_mask & data;
        data = data >> 32;
    }
    // sixth byte
    else if (address % 8 == 5)
    {
        byte_mask = byte_mask << 40;
        data = byte_mask & data;
        data = data >> 40;
    }
    // seventh byte
    else if (address % 8 == 6)
    {
        byte_mask = byte_mask << 48;
        data = byte_mask & data;
        data = data >> 48;
    }
    // eight byte
    else
    {
        byte_mask = byte_mask << 56;
        data = byte_mask & data;
        data = data >> 56;
    }

    cpu-> set_reg (rd, data);
    return;
}

// load half word unsigned
void parse_lhu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 xxx rd 0000011 
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t half_word_mask = 0x000000000000FFFF;

    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "lh rd: " << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    uint64_t address = cpu-> get_reg (rs1) + imm;

    if (address % 2 == 0)
    {
        uint64_t data = main_memory-> read_doubleword (address);

        // first half word
        if (address % 8 == 0)
        {
            data = half_word_mask & data;
        }
        // second half word
        else if (address % 8 == 2)
        {
            half_word_mask = half_word_mask << 16;
            data = half_word_mask & data;
            data = data >> 16;
        }
        // third half word
        else if (address % 8 == 4)
        {
            half_word_mask = half_word_mask << 32;
            data = half_word_mask & data;
            data = data >> 32;
        }
        // fourth half word
        else if (address % 8 == 6)
        {
            half_word_mask = half_word_mask << 48;
            data = half_word_mask & data;
            data = data >> 48;
        }
        cpu-> set_reg (rd, data);
    }
    
    // misalligned address
    else
    {
        /* throw error */
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 4); // mcause load address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }

    return ;
}

// load word unsigned
void parse_lwu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 110 rd 0000011 
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t word_mask = 0xFFFFFFFFFFFFFFFF;

    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }
    
    if (verbose)
    {
        cout << "lw rd: " << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    uint64_t address =cpu-> get_reg (rs1) + imm;
    uint64_t data = main_memory-> read_doubleword (address);

    if (address % 4 == 0)
    {
        // first word
        if (address % 8 == 0)
        {
            word_mask = word_mask >> 32;
            data = word_mask & data;
        }
        // second word
        else if (address % 8 == 4)
        {
            word_mask = word_mask << 32;
            data = word_mask & data;
            data = data >> 32;
        }
        cpu-> set_reg (rd, data);
    }
    // misalligned address
    else
    {
        /* throw error */
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 4); // mcause load address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }

    return ;
}

// fence
// fm[4] pred[4] succ[4] sssss 000 ddddd 0001111 
// parse fence - does nothing as of now
void parse_fence (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // fm[4] pred[4] succ[4] sssss 000 ddddd 0001111
    if (verbose)
    {
        cout << "fence" << endl;
    }
    return ;
}

// parse add immediate instruction
void parse_addi (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    //imm[11:0] rs1 000 rd 0010011 
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);

    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }
    
    if (verbose)
    {
        cout << "addi rd: " << dec << rd << ", rs1: " << rs1 << ", imm: " << hex << imm << endl;
    }

    cpu-> set_reg (rd, cpu-> get_reg (rs1) + imm);
    return ;
}

// parse slli
void parse_slli (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    //0000000 shamt rs1 001 rd 0011011 

    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);

    // imm value
    uint64_t imm = get_12_bit_imm (instruction);

    if (verbose)
    {
        cout << "slli rd:" << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    //slli using c++ implementation
    cpu-> set_reg (rd, (cpu-> get_reg (rs1) << imm) );
    return;
}

// slti set on less than immediate
void parse_slti (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose) 
{
    // imm[11:0] rs1 xxx rd 0010011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);
    
    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "slti rd:" << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    // set on less than
    if ((int)cpu-> get_reg (rs1) < (int)imm )
    {
        cpu-> set_reg (rd, 1);
        return;
    }
    cpu-> set_reg (rd, 0);
    return ;
}

// sltiu
void parse_sltiu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 xxx rd 0010011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);
    
    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "sltiu rd:" << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }

    // set on less than
    if ((unsigned)cpu-> get_reg (rs1) < (unsigned)imm )
    {
        cpu-> set_reg (rd, 1);
        return;
    }
    cpu-> set_reg (rd, 0);
    return ;
}

// xori
void parse_xori (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 100 rd 0010011 
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);
    
    // is neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    cpu-> set_reg (rd, cpu->get_reg(rs1) ^ imm);

    if (verbose)
    {
        cout << "xori rd:" << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }
    return ;
}

// parse srli
void parse_srli (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    //0000000 shamt rs1 101 rd 0011011 

    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);

    // imm value
    uint64_t imm = get_12_bit_imm (instruction);
    if (verbose)
    {
        cout << "srli rd:" << dec << rd << ", rs1: " << rs1 << ", imm: " << imm << endl;
    }

    cpu-> set_reg (rd, (cpu-> get_reg (rs1) >> imm) );
    return;
}

// parse srai
void parse_srai (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    //0100000 shamt rs1 101 rd 0011011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);

    // imm value
    uint64_t imm = get_12_bit_imm (instruction);
    uint64_t msb = (cpu-> get_reg (rs1) & 0x8000000000000000) >> 63;
    uint64_t data = cpu-> get_reg (rs1) >> imm;
    // if neg
    if (msb == 1)
    {
        uint64_t shifted = ( cpu-> get_reg (rs1) >> imm);
        uint64_t replacement = ~(0xFFFFFFFFFFFFFFFF >> imm );
        data = shifted | replacement;
    }

    if (verbose)
    {
        cout << "srai rd:" << dec << rd << ", rs1: " << rs1 << ", imm: " << imm << endl;
    }

    cpu-> set_reg (rd, data);
    return ;
}

// bitwise or immediate
void parse_ori (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 110 rd 0010011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);
    
    // is neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    cpu-> set_reg (rd, cpu->get_reg(rs1) | imm);

    if (verbose)
    {
        cout << "ori rd:" << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }
    return;
} 

// bitwise and immediate
void parse_andi (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:0] rs1 111 rd 0010011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);
    
    // is neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    cpu-> set_reg (rd, cpu->get_reg(rs1) & imm);

    if (verbose)
    {
        cout << "andi rd:" << dec << rd << ", rs1: " << rs1 << hex << ", imm: " << imm << endl;
    }
    return;
}

// auipc
void parse_auipc (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiii iiii iiii iiii iiii ddddd 0110111

    uint64_t rd = get_rd (instruction);

    // bits corresponding to the lui data
    uint64_t lui_mask = 0xFFFFFFFFFFFFF000; // 1111 1111 1111 1111 1111 00000 0000000
    uint64_t imm = (lui_mask & instruction);

    // if neg
    if (instruction >> 31 == 1)
    {
        imm = 0xFFFFFFFF00000000 | imm ;
    }
    imm += cpu-> get_pc();
    cpu-> set_reg (rd, imm);

    if (verbose)
    {
        cout << "auipc rd: " << dec << rd << ", imm: " << hex << imm << ", x["<< dec << rd << "]: " << hex << cpu-> get_reg (rd) << endl;
    }

    return ;
}

// parse add immediate word (addiw) instruction
void parse_addiw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    //imm[11:0] rs1 000 rd 0011011

    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);

    // if neg
    if (imm >> 11 == 1)
    {
        imm = (imm | 0xFFFFFFFFFFFFF000) ;
    }

    // the addition computation
    uint64_t data = cpu-> get_reg (rs1) + imm;
    
    if (verbose)
    {
        cout << "addiw rd: " << dec << rd << ", rs1: " << rs1 << ", imm: " << hex << imm << endl;
    }

    //if neg
    if (((data >> 31) % 2 ) == 1)
    {
        data = data & (~0xFFFFFFFF00000000);
        data = data | 0xFFFFFFFF00000000;
        cpu-> set_reg(rd, data);
        return ;
    }
    // if pos
    else
    {
        data = data & (~0xFFFFFFFF00000000);
        cpu-> set_reg(rd, data);
        return ;
    }
}

void parse_slliw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose) 
{
    //0000000 shamt rs1 001 rd 0011011 
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t shamt = get_rs2 (instruction);
    
    // if negative
    if (shamt >> 4 == 1)
    {
        shamt = shamt | 0xFFFFFFFFFFFFFF10;
    }

    // getting word
    uint64_t data = cpu-> get_reg(rs1);
    data = data & 0x00000000FFFFFFFF;
    data = data << shamt;

    // making word
    data = data & 0x00000000FFFFFFFF;

    // if negative
    if (data >> 31 == 1)
    {
        data = data | 0xFFFFFFFF00000000;
    }

    cpu-> set_reg (rd, data);

    if (verbose)
    {
        cout << "slliw rd: " << dec << rd << ", rs1: " << rs1 << ", shamt: " << hex << shamt << endl;
    }
    return ;
}

// srliw
void parse_srliw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose) 
{
    //0000000 shamt rs1 101 rd 0011011 
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t shamt = get_rs2 (instruction);
    
    // if negative
    if (shamt >> 4 == 1)
    {
        shamt = shamt | 0xFFFFFFFFFFFFFF10;
    }

    uint64_t data = cpu-> get_reg (rs1) & 0x00000000FFFFFFFF;
    data = data >> shamt;

    // getting word
    data = data & 0x00000000FFFFFFFF;

    // if negative
    if (data >> 31 == 1)
    {
        data = data | 0xFFFFFFFF00000000;
    }

    cpu-> set_reg (rd, data);

    if (verbose)
    {
        cout << "srliw rd: " << dec << rd << ", rs1: " << rs1 << ", shamt: " << hex << shamt << endl;
    }
    return ;
}

// sraiw
void parse_sraiw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose) //0100000 shamt rs1 101 rd 0011011
{
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);

    // imm value
    uint64_t shamt = get_rs2 (instruction);
    uint64_t msb = (cpu-> get_reg (rs1) & 0x00000000FFFFFFFF) >> 31;

    uint64_t data = cpu-> get_reg (rs1) & 0x00000000FFFFFFFF;
    data = data >> shamt;

    // if neg
    if (msb == 1)
    {
        uint64_t shifted = ( cpu-> get_reg (rs1) >> shamt);
        uint64_t replacement = ~(0x00000000FFFFFFFF >> shamt );
        data = (shifted | replacement) | 0xFFFFFFFF00000000;
    }

    if (verbose)
    {
        cout << "sraiw rd:" << dec << rd << ", rs1: " << rs1 << ", shamt: " << shamt << ", data: "<< ~(0x00000000FFFFFFFF >> shamt ) << endl;
    }

    cpu-> set_reg (rd, data);
    return ;
}

void parse_sb (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:5] rs2 rs1 000 imm[4:0] 0100011
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_store_imm (instruction);
    uint64_t mask = 0x00000000000000FF; // of halfword size
    uint64_t data = 0x00000000000000FF; // of halfword size
    
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "sb rs1: " << dec << rs1 << ", rs2: " << rs2 << hex <<", imm: " << imm << endl;
    }

    // checking for valid address
    uint64_t address = cpu -> get_reg (rs1) + imm;

    // shifting mask and data depending on address
    // first byte
    if (address % 8 == 0) 
    {
        data = cpu-> get_reg (rs2) & data;
    }
    // second byte
    else if (address % 8 == 1) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 8;
        mask = mask << 8;
    }
    // third byte
    else if (address % 8 == 2) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 16;
        mask = mask << 16;
    }
    // fourth byte
    else if (address % 8 == 3) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 24;
        mask = mask << 24;
    }
    // fifth byte
    else if (address % 8 == 4) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 32;
        mask = mask << 32;
    }
    // sixth byte
    else if (address % 8 == 5) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 40;
        mask = mask << 40;
    }
    // seventh byte
    else if (address % 8 == 6) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 48;
        mask = mask << 48;
    }
    // eigth byte
    else if (address % 8 == 7) 
    {
        data = cpu-> get_reg (rs2) & data; 
        data = data << 56;
        mask = mask << 56;
    }

    main_memory-> write_doubleword (address, data , mask); 
    return ;
}  

void parse_sh (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[11:5] rs2 rs1 001 imm[4:0] 0100011 
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_store_imm (instruction);
    uint64_t mask = 0x000000000000FFFF; // of halfword size
    uint64_t data = 0x000000000000FFFF; // of halfword size
    
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "sh rs1: " << dec << rs1 << ", rs2: " << rs2 << hex <<", imm: " << imm << endl;
    }

    uint64_t address = cpu -> get_reg (rs1) + imm;
    
    // checking for valid address
    if (address < 0 || address % 2 != 0)
    {
        /* throw error */
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 6); // mcause store address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }
    else
    {
        // shifting mask and data depending on address
        // first hlafword
        if (address % 8 == 0) 
        {
            data = cpu-> get_reg (rs2) & data;
        }
        // second halfword
        else if (address % 8 == 2) 
        {
            data = cpu-> get_reg (rs2) & data; 
            data = data << 16;
            mask = mask << 16;
        }
        // third halfword
        else if (address % 8 == 4) 
        {
            data = cpu-> get_reg (rs2) & data; 
            data = data << 32;
            mask = mask << 32;
        }
        // fourth halfword
        else if (address % 8 == 6) 
        {
            data = cpu-> get_reg (rs2) & data; 
            data = data << 48;
            mask = mask << 48;
        }

        main_memory-> write_doubleword (address, data , mask);  
    }

    
    return ;
}

// imm[11:5] rs2 rs1 010 imm[4:0] 0100011
// store word: M[rs1+imm] = rs2
// memory::write_doubleword (uint64_t address, uint64_t data, uint64_t mask)
void parse_sw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_store_imm (instruction);
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint64_t data = 0x00000000FFFFFFFF; // of word size
    
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "sw rs1: " << dec << rs1 << ", rs2: " << rs2 << hex <<", imm: " << imm << endl;
    }

    // checking for valid address
    uint64_t address = cpu -> get_reg (rs1) + imm;

    // missalligned address
    if (address < 0 || address % 4 != 0)
    {
        /* throw error */
        cpu->set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 6); // mcause store address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }

    else
    {
        // shifting mask and data depending on address
        // first word
        if (address % 8 == 0) 
        {
            data = cpu-> get_reg (rs2) & data;
            mask = mask >> 32;
        }

        // second word
        else if (address % 8 == 4) 
        {
            data = cpu-> get_reg (rs2) & data;
            data = data << 32;
            mask = mask << 32;
        }
        main_memory-> write_doubleword (address, data , mask); 
    }
    
    return ;
}

// store double word: M[rs1+imm] = rs2
void parse_sd (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    //imm[11:5] rs2 rs1 011 imm[4:0] 0100011
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_store_imm (instruction);
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    
    // if neg
    if (imm >> 11 == 1)
    {
        imm = imm | 0xFFFFFFFFFFFFF000;
    }

    if (verbose)
    {
        cout << "sd rs1: " << dec << rs1 << ", rs2: " << rs2 << hex <<", imm: " << imm << endl;
    }

    uint64_t address = cpu -> get_reg (rs1) + imm;

    // checking for valid address
    if (address < 0 || address % 8 != 0)
    {
        /* throw error */
        cpu-> set_csr(0x300, 0x0000000200001800); // mstatus default with MPP: 3 (machine)
        cpu-> set_csr(0x341, cpu-> get_pc() ); //mepc storring pc so can rerun instruction with alligned address
        cpu-> set_csr(0x342, 6); // mcause store address misaligned
        cpu-> set_csr(0x343, address); //mtval bad address
        cpu-> set_pc (cpu-> get_csr (0x305));// routing to trap handler address

        // as instruction is not executed
        cpu-> set_inc (false);
        cpu-> set_is_except (true);
    }
    else
    {
        // storing doubleword in memory
        main_memory-> write_doubleword (address, cpu-> get_reg (rs2) , mask); 
    }
    return ;
}

// parse add
void parse_add (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "add rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    cpu-> set_reg (rd, cpu-> get_reg (rs1) + cpu-> get_reg(rs2) );
    return ;
}

// parse sub
void parse_sub (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "sub rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    cpu-> set_reg (rd, cpu-> get_reg (rs1) - cpu-> get_reg(rs2) );
    return ;
}

// shift left logical
void parse_sll (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0000000 sssss sssss 001 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "sll rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    cpu-> set_reg (rd, cpu-> get_reg (rs1) << cpu-> get_reg(rs2) );
    return ;
}

// SLT set on less than
void parse_slt (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0000000 sssss sssss 010 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "slt rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }
    
    // set on less than
    if ((int)cpu-> get_reg (rs1) < (int)cpu-> get_reg(rs2) )
    {
        cpu-> set_reg (rd, 1 );
        return;
    }
    cpu-> set_reg (rd, 0);
    return ;
}

// SLTU set on less than unsigned
void parse_sltu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0000000 sssss sssss 011 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "sltu rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    cpu-> set_reg (rd, 0);
    // set on less than
    if (cpu-> get_reg (rs1) < cpu-> get_reg(rs2) )
    {
        cpu-> set_reg (rd, 1 );
    }
    return ;
}

// XOR
void parse_xor (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0000000 sssss sssss 100 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "xor rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }
    // bitwise xor
    cpu-> set_reg (rd, (cpu-> get_reg (rs1) ^ cpu-> get_reg(rs2) ) );
    return ;
}

// SRL shift right logical   
void parse_srl (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0000000 sssss sssss 101 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "srl rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    cpu-> set_reg (rd, cpu-> get_reg (rs1) >> cpu-> get_reg(rs2) );
    return ;
}

// SRA shift right arithmetic           
void parse_sra (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0100000 sssss sssss 101 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t msb = (cpu-> get_reg (rs1) & 0x8000000000000000) >> 63;
    uint64_t data = cpu-> get_reg (rs1) >> cpu-> get_reg (rs2);

    if (msb == 1)
    {
        uint64_t shifted = ( cpu-> get_reg (rs1) >> cpu-> get_reg (rs2));
        uint64_t replacement = ~(0xFFFFFFFFFFFFFFFF >> cpu-> get_reg (rs2) );
        data = shifted | replacement;
    }
    cpu-> set_reg (rd, data);
    
    if (verbose)
    {
        cout << "sra rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    return ;
}

// or
void parse_or (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0100000 sssss sssss 110 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "or rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }
    // bitwise or
    cpu-> set_reg (rd, (cpu-> get_reg (rs1) | cpu-> get_reg(rs2) ) );
    return ;
}

// and
void parse_and (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // 0100000 sssss sssss 111 rrrrr 0110011
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "and rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }
    // bitwise or
    cpu-> set_reg (rd, (cpu-> get_reg (rs1) & cpu-> get_reg(rs2) ) );
    return ;
}

// parse lui
void parse_lui (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiii iiii iiii iiii iiii ddddd 0110111

    uint64_t rd = get_rd (instruction);
    uint64_t imm = instruction;

    // last digits are zero
    imm = imm & 0xFFFFF000;
    if (verbose)
    {
        cout << "lui rd: " << dec << rd << " imm " << hex << imm << endl;
    }

    // if neg
    if ((imm >> 31) == 1)
    {
        imm = imm | 0xFFFFFFFF00000000;
    }


    cpu-> set_reg (rd, imm );
    return ;
}

// addw
void parse_addw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "addw rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    // operation
    uint64_t data = cpu-> get_reg (rs1) + cpu-> get_reg (rs2);
    data = data & 0x00000000FFFFFFFF;
    if (data >> 31 == 1)
    {
        data = 0xFFFFFFFF00000000 | data;
    }

    cpu-> set_reg (rd, data);
    return ;
}

// subw
void parse_subw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    if (verbose)
    {
        cout << "subw rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }

    // operation
    uint64_t data = cpu-> get_reg (rs1) - cpu-> get_reg (rs2);
    data = data & 0x00000000FFFFFFFF;
    if (data >> 31 == 1) // if neg
    {
        data = 0xFFFFFFFF00000000 | data;
    }

    cpu-> set_reg (rd, data);
    return ;
}

/* problem */
// SLLW - shift left logical word
void parse_sllw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    uint64_t data = (cpu-> get_reg(rs1) & 0x00000000FFFFFFFF) << (cpu-> get_reg(rs2) & 0x00000000FFFFFFFF);
    // getting word
    data = data & 0x00000000FFFFFFFF;

    // if negative
    if (data >> 31 == 1)
    {
        data = data | 0xFFFFFFFF00000000;
    }

    cpu-> set_reg (rd, data);

    if (verbose)
    {
        cout << "sllw rd: " << dec << rd << ", rs1: " << rs1 << ", rs2: " << hex << rs2 << endl;
    }
    return ;
}

/* problem */
// shift right logical word
void parse_srlw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);

    uint64_t data = (cpu-> get_reg(rs1) & 0x00000000FFFFFFFF) >> (cpu-> get_reg(rs2) & 0x00000000FFFFFFFF);
    // getting word
    data = data & 0x00000000FFFFFFFF;

    // if negative
    if (data >> 31 == 1)
    {
        data = data | 0xFFFFFFFF00000000;
    }

    cpu-> set_reg (rd, data);

    if (verbose)
    {
        cout << "sllw rd: " << dec << rd << ", rs1: " << rs1 << ", rs2: " << hex << rs2 << endl;
    }
    return ;
}

/* problem */
// shift right arithmatic word
void parse_sraw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rd = get_rd (instruction) ;
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t data = (cpu-> get_reg (rs1) & 0x00000000FFFFFFFF) >> (cpu-> get_reg (rs2) & 0x00000000FFFFFFFF);

    if (cpu-> get_reg (rs1) >> 63)
    {
        uint64_t replacement = ~(0xFFFFFFFFFFFFFFFF >> cpu-> get_reg (rs2) );
        data = data | replacement;
    }

    cpu-> set_reg (rd, data);
    
    if (verbose)
    {
        cout << "sraw rd:" << dec << rd << ", rs1: " << rs1 << ", rs2: " << rs2 << endl;
    }
    return ;
}

// branch equal to
void parse_beq (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_branch_imm (instruction);
    uint64_t msb = (imm & 0xFFF) >> 11;
    if (msb == 1) 
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12;
    }

    if (verbose)
    {
        cout << dec << "beq rs1: " << rs1 << ", rs2: " << rs2 << ", imm: " << hex << imm << endl;
    }

    // beq command
    if (cpu-> get_reg (rs1) == cpu-> get_reg(rs2) )
    {
        cpu-> set_inc (false);
        cpu-> set_pc (cpu-> get_pc() + imm);
        return ;
    }

    cpu-> set_inc (true);
    return ;
}

// parse bne
void parse_bne (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{

    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_branch_imm (instruction);
    uint64_t msb = (imm & 0xFFF) >> 11;

    if (msb == 1) 
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12;
    }

    if (verbose)
    {
        cout << dec << "bne rs1: " << rs1 << ", rs2: " << rs2 << ", imm: " << hex << imm << endl;
    }

    // bne command
    if (cpu-> get_reg (rs1) != cpu-> get_reg(rs2) )
    {
        cpu-> set_inc (false);
        cpu-> set_pc (cpu-> get_pc() + imm);
        return ;
    }

    cpu-> set_inc (true);
    return ;
}

// branch less than (with a cast)
void parse_blt (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_branch_imm (instruction);
    uint64_t msb = (imm & 0xFFF) >> 11;
    if (msb == 1) 
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12;
    }

    if (verbose)
    {
        cout << dec << "blt rs1: " << rs1 << ", rs2: " << rs2 << ", imm: " << hex << imm << endl;
    }

    // beq command
    if ( (int) cpu-> get_reg (rs1) < (int) cpu-> get_reg(rs2) )
    {
        cpu-> set_inc (false);
        cpu-> set_pc (cpu-> get_pc() + imm);
        return ;
    }

    cpu-> set_inc (true);
    return ;
}

// branch greater than equal
void parse_bge (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_branch_imm (instruction);
    uint64_t msb = (imm & 0xFFF) >> 11;
    if (msb == 1) 
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12;
    }

    if (verbose)
    {
        cout << dec << "bge rs1: " << rs1 << ", rs2: " << rs2 << ", imm: " << hex << imm << endl;
    }

    // beq command (with a cast)
    if ( (int) cpu-> get_reg (rs1) >= (int) cpu-> get_reg(rs2) )
    {
        cpu-> set_inc (false);
        cpu-> set_pc (cpu-> get_pc() + imm);
        return ;
    }

    cpu-> set_inc (true);
    return ;
}

// branch less than unsigned
void parse_bltu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_branch_imm (instruction);
    uint64_t msb = (imm & 0xFFF) >> 11;
    if (msb == 1) 
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12;
    }

    if (verbose)
    {
        cout << dec << "bltu rs1: " << rs1 << ", rs2: " << rs2 << ", imm: " << hex << imm << endl;
    }

    // beq command
    if (cpu-> get_reg (rs1) < cpu-> get_reg(rs2) )
    {
        cpu-> set_inc (false);
        cpu-> set_pc (cpu-> get_pc() + imm);
        return ;
    }

    cpu-> set_inc (true);
    return ;
}

// branch greater than unsigned
void parse_bgeu (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t rs2 = get_rs2 (instruction);
    uint64_t imm = get_branch_imm (instruction);
    uint64_t msb = (imm & 0xFFF) >> 11;
    if (msb == 1) 
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12;
    }

    if (verbose)
    {
        cout << dec << "bgeu rs1: " << rs1 << ", rs2: " << rs2 << ", imm: " << hex << imm << endl;
    }

    // beq command
    if (cpu-> get_reg (rs1) >= cpu-> get_reg(rs2) )
    {
        cpu-> set_inc (false);
        cpu-> set_pc (cpu-> get_pc() + imm);
        return ;
    }

    cpu-> set_inc (true);
    return ;
}

// parse jalr
/* problem */
void parse_jalr (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // iiiiiiiiiiii sssss 000 ddddd 1100111
    uint64_t rd = get_rd (instruction);
    uint64_t rs1 = get_rs1 (instruction);
    uint64_t imm = get_12_bit_imm (instruction);

    // link
    if (rd != 0)
        cpu-> set_reg (rd, cpu-> get_pc() + 4) ;

    // if neg
    if (imm >> 11 == 1) // msb
    {
        uint64_t neg_mask_12 =  0xFFFFFFFFFFFFF000; // 1...1 0000 0000 0000
        imm = imm | neg_mask_12 ;
    }

    // jump
    uint64_t temp_pc = cpu-> get_reg (rs1) + imm ;
    temp_pc -= temp_pc % 2;
    cpu-> set_pc (temp_pc);
    
    cpu-> set_inc (false);

    if (verbose)
    {
        cout << "jalr " << dec << "rd :" << rd << " rs1: " << rs1 << " imm: " << hex << imm << endl;
    }
    return ;
}

// jal
void parse_jal (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // imm[20|10:1|11|19:12] rrrrr 1101111
    uint64_t rd = get_rd (instruction);
    
    // imm[20]
    uint64_t imm_20 = instruction & 0x80000000;
    imm_20 = imm_20 >> 31;
    imm_20 = imm_20 << 20;

    // imm[10:1]
    uint64_t imm_low = instruction & 0x7FE00000; // 0 1111111111 0 00000000 00000 0000000
    imm_low = imm_low >> 21;
    imm_low = imm_low << 1;
    
    // imm[11]
    uint64_t imm_11 = instruction & 0x100000; // 0 0000000000 1 00000000 00000 0000000
    imm_11 = imm_11 >> 20;
    imm_11 = imm_11 << 11;


    // imm[19:12]
    uint64_t imm_high = instruction & 0xFF000; // 0 0000000000 0 11111111 00000 0000000
    // imm_high = imm_high >> 12;
    // imm_high = imm_high << 12;

    uint64_t imm = imm_20 + imm_high + imm_11 + imm_low;
    // if neg
    if (imm >> 20 == 1)
    {
        imm = 0xFFFFFFFFFFF00000 | imm ;
    }

    cpu-> set_reg (rd, cpu-> get_pc () + 4);
    cpu-> set_pc (cpu-> get_pc () + imm);
    cpu-> set_inc (false);
    if (verbose)
    {
        cout << "jal rd: " << dec << rd << ", imm: " << hex << imm << endl;
    }

    return ;
}

//csr rs1 001 rd 1110011 CSRRW
void parse_csrrw (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // uint64_t rs1 = get_rs1(instruction);
    // uint64_t rd = get_rd (instruction);
    // uint64_t csr = get_12_bit_imm (instruction);

    return ;
}

//csr rs1 010 rd 1110011 CSRRS
void parse_csrrs (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // uint64_t rs1 = get_rs1(instruction);
    // uint64_t rd = get_rd (instruction);
    // uint64_t csr = get_12_bit_imm (instruction);
    return ;
}

//csr rs1 011 rd 1110011 CSRRC
void parse_csrrc (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // uint64_t rs1 = get_rs1(instruction);
    // uint64_t rd = get_rd (instruction);
    // uint64_t csr = get_12_bit_imm (instruction);
    return ;
}

//csr uimm 101 rd 1110011 CSRRWI
void parse_csrrwi (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // uint64_t uimm = get_rs1(instruction);
    // uint64_t rd = get_rd (instruction);
    // uint64_t csr = get_12_bit_imm (instruction);
    return ;
}

//csr uimm 110 rd 1110011 CSRRSI
void parse_csrrsi (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // uint64_t uimm = get_rs1(instruction);
    // uint64_t rd = get_rd (instruction);
    // uint64_t csr = get_12_bit_imm (instruction);
    return ;
}

//csr uimm 111 rd 1110011 CSRRCI
void parse_csrrci (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // uint64_t uimm = get_rs1(instruction);
    // uint64_t rd = get_rd (instruction);
    // uint64_t csr = get_12_bit_imm (instruction);
    return ;
}

// ecall: environment call
void parse_ecall (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // exception doesn't count as execution
    cpu-> set_inc (false);
    cpu-> set_is_except (true);

    // things that change regardless of prv
    cpu-> set_csr (0x341, cpu-> get_pc ()); // mepc storing pc to come back to later
    cpu-> set_csr (0x343, 0); // mtval no dodgey address or value to return

    // if U
    if(cpu-> get_prv() == 0)
    {
        cpu-> set_csr (0x300, 0x0000000200000000); // mstatus MPP:0
        cpu-> set_csr (0x342, 8); //mcause
        cpu-> set_pc (cpu->get_csr(0x305)); //mtvec
    }
    // if M
    else if(cpu-> get_prv() == 3)
    {
        cpu-> set_csr (0x300, 0x0000000200001800); // mstatus // MPP:3 (1100000000000)
        cpu-> set_csr (0x342, 11); //mcause
        cpu-> set_pc (cpu-> get_csr (0x305)); //mtvec
    }
    return ;
}

// ebreak: for breakpoint
void parse_ebreak (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    cpu->set_is_except (true);
    cpu->set_inc (false);
    if (cpu-> get_prv() == 3)
    {
        // if mie = 1
        if( (cpu-> get_reg (0x300) & 0x0000000200000008) == 0x0000000200000008)
        {
            cpu->set_csr(0x300,0x0000000200001880);
        }
        else
        {
            // else set to deafult with mpp = 3
            cpu->set_csr(0x300,0x0000000200001800);
        }
    } 
    else if (cpu-> get_prv() == 0)
    {
        // if mie
        if( (cpu-> get_reg (0x300) & 0x0000000200000008) == 0x0000000200000008)
        {
            // mpie set
            cpu->set_csr(0x300,0x0000000200000080);
        }
        else
        {
            // else 0 mpp and mpie
            cpu->set_csr(0x300,0x0000000200000000);
        }
    }
    
    cpu-> set_pc (cpu-> get_csr (0x305)); //mtvec
    cpu-> set_reg (0x341, cpu-> get_pc ()); //mepc
    cpu-> set_reg (0x342, 3); // mcause
    cpu-> set_prv (3);

    return ;
}

// parse risc-v 64 instruction
void parse_instruction (uint64_t instruction, memory* main_memory, processor* cpu, bool verbose)
{
    // opcode and function 3
    uint64_t opcode = get_opcode (instruction);
    uint64_t fn3 = get_fn3 (instruction);
    
    // loading data
    if (opcode == 3)
    {
        // ld
        if (fn3 == 0)
        {
            // load byte
            parse_lb (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 1)
        {
            // load half word
            parse_lh (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 2)
        {
            // load word
            parse_lw (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 3)
        {
            // load double word
            parse_ld (instruction, main_memory, cpu, verbose); 
        }
        else if (fn3 == 4)
        {
            // load word unsigned
            parse_lbu (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 5)
        {
            // load half word unsigned
            parse_lhu (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 6)
        {
            // load word unsigned
            parse_lwu (instruction, main_memory, cpu, verbose); // imm[11:0] rs1 110 rd 0000011 
        }
    }

    // fence
    else if (opcode == 15 && fn3 == 0) // fm[4] pred[4] succ[4] sssss 000 ddddd 0001111 
    {
        parse_fence (instruction, main_memory, cpu, verbose);
    }

    // addi / shift
    else if (opcode == 19) 
    {
        // addi
        if (fn3 == 0)
        {
            parse_addi (instruction, main_memory, cpu, verbose); //imm[11:0] rs1 000 rd 0010011 
        }
        else if (fn3 == 1) 
        {
            uint64_t gimped_fn7 = get_fn7 (instruction);
            gimped_fn7 = gimped_fn7 >> 1;
            if (gimped_fn7 == 0)
            {
                // slli
                parse_slli (instruction, main_memory, cpu, verbose); // 000000 iiiiii sssss 001 ddddd 0010011
            }
        }
        else if (fn3 == 2) 
        {
            // slti
            parse_slti (instruction, main_memory, cpu, verbose); // imm[11:0] rs1 010 rd 0010011
        }
        else if (fn3 == 3) 
        {
            // sltiu
            parse_sltiu (instruction, main_memory, cpu, verbose); // imm[11:0] rs1 011 rd 0010011 
        }
        else if (fn3 == 4) 
        {
            // xori
            parse_xori (instruction, main_memory, cpu, verbose); // imm[11:0] rs1 100 rd 0010011 
        }
        // shift right
        else if (fn3 == 5) 
        {
            uint64_t gimped_fn7 = get_fn7 (instruction);
            gimped_fn7 = gimped_fn7 >> 1;
            if (gimped_fn7 == 0)
            {
                // srli
                parse_srli (instruction, main_memory, cpu, verbose); // 0000000 shamt rs1 101 rd 0010011
            }
            else if (gimped_fn7 == 16)
            {
                // srai
                parse_srai (instruction, main_memory, cpu, verbose); // 0100000 shamt rs1 101 rd 0010011 
            }
        }
        else if (fn3 == 6)
        {
            parse_ori (instruction, main_memory, cpu, verbose); // imm[11:0] rs1 110 rd 0010011
        }
        else if (fn3 == 7)
        {
            // bitwise and immediate
            parse_andi (instruction, main_memory, cpu, verbose); // imm[11:0] rs1 111 rd 0010011
        }
    }

    // auipc
    else if (opcode == 23)
    {
        parse_auipc (instruction, main_memory, cpu, verbose);
    }

    else if (opcode == 27)
    {
        if (fn3 == 0)
        {
            // addiw
            parse_addiw (instruction, main_memory, cpu, verbose); //imm[11:0] rs1 000 rd 0011011 
        }
        else if (fn3 == 1)
        {
            uint64_t fn7 = get_fn7 (instruction);
            if (fn7 == 0)
            {
               // slliw
                parse_slliw (instruction, main_memory, cpu, verbose); //0000000 shamt rs1 001 rd 0011011 
            }
        }
        else if (fn3 == 5)
        {
            uint64_t fn7 = get_fn7 (instruction);
            if (fn7 == 0)
            {
                // srliw
                parse_srliw (instruction, main_memory, cpu, verbose); //0000000 shamt rs1 101 rd 0011011 
            }
            else if (fn7 == 32)
            {
                // sraiw
                parse_sraiw (instruction, main_memory, cpu, verbose); //0100000 shamt rs1 101 rd 0011011
            }
            
        }
        
    }
    // store
    else if (opcode == 35) // 0011011 && 000
    {
        if (fn3 == 0)
        {
            parse_sb (instruction, main_memory, cpu, verbose); // imm[11:5] rs2 rs1 000 imm[4:0] 0100011 
        }
        else if (fn3 == 1)
        {
            parse_sh (instruction, main_memory, cpu, verbose); // imm[11:5] rs2 rs1 001 imm[4:0] 0100011 
        }
        else if (fn3 == 2)
        {
            parse_sw (instruction, main_memory, cpu, verbose); // imm[11:5] rs2 rs1 010 imm[4:0] 0100011 
        }
        else if (fn3 == 3)
        {
             parse_sd (instruction, main_memory, cpu, verbose); //imm[11:5] rs2 rs1 011 imm[4:0] 0100011
         }
     }
    // math
     else if (opcode == 51)
     {
        // add or sub
        if (fn3 == 0)
        {
            uint64_t fn7 = get_fn7 (instruction);

            // add
            if (fn7 == 0) 
            {
                parse_add (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 000 ddddd 0110011
            }
            // sub
            else if (fn7 == 32)
            {
                parse_sub (instruction, main_memory, cpu, verbose); // 0100000 sssss sssss 000 ddddd 0110011
            }
        }
        else if (fn3 == 1)
        {
            // SLL shift left logical
            parse_sll (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 001 rrrrr 0110011
        }
        else if (fn3 == 2)
        {
            // SLT set on less than
            parse_slt (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 010 rrrrr 0110011
        }
        else if (fn3 == 3)
        {
            // SLTU set on less than unsigned
            parse_sltu (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 011 rrrrr 0110011
        }
        else if (fn3 == 4)
        {
            // XOR
            parse_xor (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 100 rrrrr 0110011
        }
        else if (fn3 == 5)
        {
            // shift right
            uint64_t fn7 = get_fn7 (instruction);

            // SRL shift right logical
            if (fn7 == 0)
            {
                parse_srl (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 101 rrrrr 0110011
            }
            // SRA shift right arithmetic
            else if (fn7 == 32)
            {
                parse_sra (instruction, main_memory, cpu, verbose); // 0100000 sssss sssss 101 rrrrr 0110011
            }
        }
        else if (fn3 == 6)
        {
            uint64_t fn7 = get_fn7 (instruction);
            if (fn7 == 0)
            {
                // bitwise OR
                parse_or (instruction, main_memory, cpu, verbose); // 0100000 sssss sssss 110 rrrrr 0110011
            }

        }
        else if (fn3 == 7)
        {   
            uint64_t fn7 = get_fn7 (instruction);
            if (fn7 == 0)
            {
                //bitwise AND
                parse_and (instruction, main_memory, cpu, verbose); // 0000000 sssss sssss 111 rrrrr 0110011
            }
        }
    }


    // lui
    else if (opcode == 55) // iiii iiii iiii iiii iiii ddddd 0110111
    {
        parse_lui (instruction, main_memory, cpu, verbose);
    }

    // add / sub / shift words
    else if (opcode == 59)
    {
        uint64_t fn7 = get_fn7 (instruction);

        // add / sub
        if  (fn3 == 0)
        {
            if (fn7 == 0)
            {
                // addw
                parse_addw (instruction, main_memory, cpu, verbose);
            }
            else if (fn7 == 32)
            {
                // subw
                parse_subw (instruction, main_memory, cpu, verbose);
            }
        }

        // shift left word
        else if (fn3 == 1)
        {
            if (fn7 == 0)
            {
                // SLLW - shift left logical word
                parse_sllw (instruction, main_memory, cpu, verbose);
            }
        }

        // shift right word
        else if (fn3 == 5)
        {
            if (fn7 == 0)
            {
                // shift right logical word
                parse_srlw (instruction, main_memory, cpu, verbose);
            }
            else if (fn7 == 32)
            {
                // shift right arithmatic word
                parse_sraw (instruction, main_memory, cpu, verbose);
            }
        }
    }
    
    // branching
    else if (opcode == 99) 
    {
        // branch equal
        if (fn3 == 0)
        {
            parse_beq (instruction, main_memory, cpu, verbose);
        }
        // branch not equal
        else if (fn3 == 1)
        {
            parse_bne (instruction, main_memory, cpu, verbose);
        }
        // branch less than
        else if (fn3 == 4)
        {
            parse_blt (instruction, main_memory, cpu, verbose);
        }
        // branch greater than equal
        else if (fn3 == 5)
        {
            parse_bge (instruction, main_memory, cpu, verbose);
        }
        // branch less than unsigned
        else if (fn3 == 6)
        {
            parse_bltu (instruction, main_memory, cpu, verbose);
        }
        // branch greater than unsigned
        else if (fn3 == 7)
        {
            parse_bgeu (instruction, main_memory, cpu, verbose);
        }
    }

    // jalr
    else if (opcode == 103 && fn3 == 0) // imm[11:0] sssss 000 rrrrr 1100111 
    {
        parse_jalr (instruction, main_memory, cpu, verbose);
    }

    // jal
    else if (opcode == 111) // imm[20|10:1|11|19:12] rrrrr 1101111
    {
        parse_jal (instruction, main_memory, cpu, verbose);
    }

    //ecall or ebreak
    else if (opcode == 115)
    {
        if (fn3 == 0)
        {
            // ecall
            if (instruction == 0x73) // 000000000000 00000 000 00000 1110011 
            {
                parse_ecall (instruction, main_memory, cpu, verbose);
            }

            // ebreak
            else if (instruction == 0x100073) // 000000000001 00000 000 00000 1110011 
            {
                parse_ebreak (instruction, main_memory, cpu, verbose);
            }
        }
        
        else if (fn3 == 1)
        {
            //csr rs1 001 rd 1110011 CSRRW
            parse_csrrw (instruction, main_memory, cpu, verbose);
        }

        else if (fn3 == 2)
        {
            //csr rs1 010 rd 1110011 CSRRS
            parse_csrrs (instruction, main_memory, cpu, verbose);
        }

        else if (fn3 == 3)
        {
            //csr rs1 011 rd 1110011 CSRRC
            parse_csrrc (instruction, main_memory, cpu, verbose);
        }

        else if (fn3 == 5)
        {
            //csr uimm 101 rd 1110011 CSRRWI
            parse_csrrwi (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 6)
        {
            //csr uimm 110 rd 1110011 CSRRSI
            parse_csrrsi (instruction, main_memory, cpu, verbose);
        }
        else if (fn3 == 7)
        {
            //csr uimm 111 rd 1110011 CSRRCI
            parse_csrrci (instruction, main_memory, cpu, verbose);
        }
    }

    // bad input
    else
    {
        cout << "illegal instruction" << endl;
    }
}