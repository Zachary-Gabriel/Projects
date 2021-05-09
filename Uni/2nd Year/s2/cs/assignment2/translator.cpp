// convert a document tree into VM code
#include "iobuffer.h"
#include "abstract-syntax-tree.h"
#include "assembler-internal.h"

// to make out programs a bit neater
using namespace std ;

using namespace CS_IO_Buffers ;
using namespace Hack_Virtual_Machine ;

// grammer to be parsed:
// vm_class ::= command* eoi
// command ::= vm_operator | vm_jump | vm_func | vm_stack
// vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
// vm_jump ::= jump label
// vm_func ::= func label number
// vm_stack ::= stack segment number

// forward declare parsing functions - one per rule
static void translate_vm_class(ast root) ;
static void translate_vm_command(ast command) ;
static void translate_vm_operator(ast vm_op) ;
static void translate_vm_jump(ast jump) ;
static void translate_vm_func(ast func) ;
static void translate_vm_stack(ast stack) ;
static string current_function ;
static int var = 1;
// the function translate_vm_class() will be called by the main program
// its is passed the abstract syntax tree constructed by the parser
// it walks the abstract syntax tree and produces the equivalent VM code as output
static void translate_vm_class(ast root)
{
    // assumes we have a "class" node containing VM command nodes
    ast_mustbe_kind(root,ast_vm_class) ;

    // tell the output system we are starting to translate VM commands for a Jack class
    start_of_vm_class() ;

    int ncommands = size_of_vm_class(root) ;
    for ( int i = 0 ; i < ncommands ; i++ )
    {
        translate_vm_command(get_vm_class(root,i)) ;
    }

    // tell the output system we have just finished translating VM commands for a Jack class
    end_of_vm_class() ;

}

// HELPER FUNCTIONS

// parses the pushes that refer to creating a whole new stack
void parse_simple_push(string location)
{
    // get the index of what we want and put it in D
    output_assembler ("@" + location ) ;
    output_assembler ("D=M") ;

    // find the location of where the next stack should be
    output_assembler ("@SP") ;
    output_assembler ("A=M") ;

    // then add D to this stack
    output_assembler ("M=D") ;

    // then increment the stack size by one
    output_assembler ("@SP") ;
    output_assembler ("M=M+1");
}

// parses the pushes that refer to a specific stack (usually inside one of the first 5)
void parse_complex_push(string location, string segment)
{   
    // finding the lcl pointer
    output_assembler ("@" + segment);
    output_assembler ("D=M") ;

    // incrementing the pointer by the input value
    output_assembler ("@" + location ) ;
    output_assembler ("A=D+A") ;
    output_assembler ("D=M") ;

    output_assembler ("@SP") ; 
    output_assembler ("A=M") ; // get the index of where the stack's at
    output_assembler ("M=D") ; // set that value to the input

    // increment the stack size by one
    output_assembler ("@SP") ;
    output_assembler ("M=M+1") ;
}

// parses the pops that only refer to poping a whole stack
void parse_simple_pop (string location)
{
    output_assembler ("@SP") ;
    output_assembler ("M=M-1") ; // going to most recent addition to stack
    output_assembler ("A=M") ; // set A to this
    output_assembler ("D=M") ; // set D to the value of ram[A]
    output_assembler ("@" + location ) ; // set A to the location of where the pop is going
    output_assembler ("M=D") ; // and make ram[A] = to the value that was popped off
}

// popping elements off a specific stack
void parse_complex_pop (string location, string segment)
{
    output_assembler ("@" + segment) ; // loading this onto A
    output_assembler ("D=M") ; // storing it in the D register

    output_assembler ("@" + location) ; // loading the number
    output_assembler ("D=D+A") ; // incrementing the D register's value by A

    output_assembler ("@R13") ; // storing the pop location inside one of the general purpose memory cells
    output_assembler ("M=D") ;

    output_assembler ("@SP") ; // loading the stack pointer
    output_assembler ("M=M-1") ; // reducing the stack size by one
    output_assembler ("A=M") ; // read the pointer that has just been popped
    output_assembler ("D=M") ; // getting the data we want at the top of the stack

    output_assembler ("@R13") ; // grabbing the location we want to modify
    output_assembler ("A=M") ; 
    output_assembler ("M=D") ; // changing the data at ram[ram[1 -> 4] + number] to the value we popped off the stack earlier
}

// returns output for goto
void parse_goto (string label)
{
   output_assembler ("@" + current_function + label) ; // load the address of the label onto A
   output_assembler ("0;JMP") ; // then do a jump (to A) regardless of anything
}

// return output for if-goto
void parse_if_goto(string label)
{
    output_assembler ("@SP") ; // go to the stack
    output_assembler ("M=M-1") ; // pop off the stack
    output_assembler ("A=M") ; // set A to be the thing getting popped off
    output_assembler ("D=M") ; // set D to be Ram[A]

    output_assembler ("@" + current_function + label) ; // load the location of the label onto A
    output_assembler ("D;JNE") ; // jump to label if not equal
}

// translate the current vm command - a bad node is a fatal error
static void translate_vm_command(ast command)
{
    switch(ast_node_kind(command))
    {
        case ast_vm_operator:
        translate_vm_operator(command) ;
        break ;
        case ast_vm_jump:
        translate_vm_jump(command) ;
        break ;
        case ast_vm_function:
        translate_vm_func(command) ;
        break ;
        case ast_vm_stack:
        translate_vm_stack(command) ;
        break ;
        default:
        fatal_error(0,"// bad node - expected vm_operator, vm_jump, vm_function or vm_stack\n") ;
        break ;
    }
}

// translate vm operator command into assembly language
static void translate_vm_operator(ast vm_op)
{
    // extract command specific info from the ast node passed in
    string the_op = get_vm_operator_command(vm_op) ;

    // tell the output system what kind of VM command we are now trying to implement
    start_of_vm_operator_command(the_op) ;

    if (the_op == "add")
    {
        output_assembler ("@SP") ; // loads stack pointer onto A
        output_assembler ("M=M-1") ; // gets the index of the most recent addition onto the stack
        output_assembler ("A=M") ; // sets A to this index
        output_assembler ("D=M") ; // make D equal to the most recent addition to the stack (will be b for a+b)
        output_assembler ("A=A-1") ; // reduce the index by one (as the function add:R->R is a function that reduces two inputs to one (we also want the value of a for a+b))
        output_assembler ("M=D+M") ; // set the value of a in a+b to a+b
    }

    if (the_op == "sub")
    {
        output_assembler ("@SP") ; // loads stack pointer onto A
        output_assembler ("M=M-1") ; // gets the index of the most recent addition onto the stack
        output_assembler ("A=M") ; // sets A to this index
        output_assembler ("D=M") ; // make D equal to the most recent addition to the stack (will be b for a-b)
        output_assembler ("A=A-1") ; // reduce the index by one (as the function sub:R->R is a function that reduces two inputs to one (we also want the value of a for a-b))
        output_assembler ("M=M-D") ; // set the value of a in a-b to a-b and stores it in memory
    }

    if (the_op == "neg")
    {
        output_assembler ("@SP") ;
        output_assembler ("A=M") ; // get the most recent thing on the stack
        output_assembler ("A=A-1") ;
        output_assembler ("M=-M") ; // set that value to negative itself
    }

    if (the_op == "and")
    {
        output_assembler ("@SP") ;
        output_assembler ("M=M-1") ; // get most recent thing on the stack (and delete it)
        output_assembler ("A=M") ;
        output_assembler ("D=M") ; // set D = most recent thing on the stack
        output_assembler ("A=A-1") ; // get the next most recent thing on the stack
        output_assembler ("M=D&M") ; // put the rest of the a&b where a = ram[sp-1] & b = ram[sp-2] 
    }

    if (the_op == "or")
    {
        output_assembler ("@SP") ;
        output_assembler ("M=M-1") ; // get most recent thing on the stack (and delete it)
        output_assembler ("A=M") ;
        output_assembler ("D=M") ; // set D = most recent thing on the stack
        output_assembler ("A=A-1") ; // get the next most recent thing on the stack
        output_assembler ("M=D|M") ; // put the rest of the a|b where a = ram[sp-1] | b = ram[sp-2] 
    }

    if (the_op == "not")
    {
        output_assembler ("@SP") ;
        output_assembler ("A=M") ; // get most recent thing on the stack
        output_assembler ("A=A-1") ;
        output_assembler ("M=!M") ; // set it to not itself
    }

    if (the_op == "return" || the_op == "eq" || the_op == "gt" || the_op == "lt") // haven't done yet
    {
        output_assembler ("@SP") ; 
    }

    // tell the output system that we have just finished trying to implement a VM command
    end_of_vm_command() ;
}

// translate vm operator command into assembly language
static void translate_vm_jump(ast jump)
{
    // extract command specific info from the ast node passed in
    string command = get_vm_jump_command(jump) ;
    string label = get_vm_jump_label(jump) ;

    // tell the output system what kind of VM command we are now trying to implement
    start_of_vm_jump_command(command,label) ;

    if (command == "goto" ) 
    {
        parse_goto (label); // to save code for call function
    }

    if (command ==  "if-goto") 
    {
        parse_if_goto(label) ; // to save code for call function
    }

    if (command ==  "label") output_assembler("(" + current_function + label +")") ; // make a fancy label

    // tell the output system that we have just finished trying to implement a VM command
    end_of_vm_command() ;
}

// translate vm operator command into assembly language
static void translate_vm_func(ast func)
{
    // extract command specific info from the ast node passed in
    string command = get_vm_function_command(func) ;
    string label = get_vm_function_label(func) ;
    int number = get_vm_function_number(func) ;

    start_of_vm_func_command(command,label,number) ;
    if (command == "function")
    {
        // doing a global label for the function
        output_assembler("("+label+")") ;

        // to set the labels correctly when doing a jump command inside a function
        current_function = label+"$"; 

        // doing the push pop thing for the amount of local variables the function has
        for (int i = 0 ; i < number; ++i) 
        {
            output_assembler ("@SP") ; 
            output_assembler ("M=M+1") ; // pushing "constant" onto the stack
            output_assembler ("A=M") ; // setting A to the index of the next thing on the stack
            output_assembler ("A=A-1") ; // then going back one (so that we can play with the most recent addition)
            output_assembler ("M=0") ; // setting constant to 0
        }
    }

    if (command == "call")
    {
        ++var; // for uniqueness
        // as this is more like const than the others that are parsed in the function
        
        // store the variable in D
        output_assembler ("@" + current_function + label + to_string(var) );
        
        output_assembler ("D=A") ; 

        // move the variable to the stakc
        output_assembler ("@SP") ;
        output_assembler ("A=M") ;
        output_assembler ("M=D") ;

        // increment the stack size
        output_assembler ("@SP") ;
        output_assembler ("M=M+1") ;

        for (unsigned int i = 1; i < 5; ++i)
        {
            parse_simple_push (to_string (i) ) ; // saves the lcl, arg, this and that of f (pushes @ 0->4 to the stack)
        }

        // ARG = SP-nArgs-5
        output_assembler ("@" + to_string (number) ) ; 
        output_assembler ("D=A") ; // D = nArgs 
        
        output_assembler ("@5") ;
        output_assembler ("D=D+A") ; // D = 5+ nargs
        output_assembler ("D=-D") ; // D = -5 -nArgs 
        
        output_assembler ("@SP") ;
        output_assembler ("D=D+M") ; // the pointer of sp -5-nargs
        output_assembler ("@ARG") ; // ARG = sp -5-nargs
        output_assembler ("M=D") ; // problems

        // LCL = SP
        output_assembler ("@SP") ;
        output_assembler ("D=M") ; // store sp in D
        output_assembler ("@LCL") ;
        output_assembler ("M=D") ; // set lcl to D

        // transfers control to g
        output_assembler ("@" + label) ; 
        output_assembler ("0;JMP") ; 

        output_assembler ("(" + current_function + label + to_string(var)+")"); // the generated label
    }

    // tell the output system that we have just finished trying to implement a VM command
    end_of_vm_command() ;
}

// translate vm operator command into assembly language
static void translate_vm_stack(ast stack)
{
    // extract command specific info from the ast node passed in
    string command = get_vm_stack_command(stack) ;
    string segment = get_vm_stack_segment(stack) ;
    int number = get_vm_stack_offset(stack) ;

    // tell the output system what kind of VM command we are now trying to implement
    start_of_vm_stack_command(command,segment,number) ;

    if (command == "push")
    {

        if (segment == "constant") 
        {
            // store the variable in D
            output_assembler ("@" + to_string (number) ) ;
            output_assembler ("D=A") ; 

            // move the variable to the stakc
            output_assembler ("@SP") ;
            output_assembler ("A=M") ;
            output_assembler ("M=D") ;

            // increment the stack size
            output_assembler ("@SP") ;
            output_assembler ("M=M+1") ;
        }

        if (segment == "static")
        {
            parse_simple_push (to_string (number + 16) );
        }

        if (segment == "temp")
        {
            parse_simple_push (to_string (number + 5) );
        }

        if (segment == "pointer") // only for changing this and that themselves
        {
            parse_simple_push (to_string (number + 3) );
        }
        
        if (segment == "local")
        {   
            parse_complex_push (to_string (number), "1");
        }

        if (segment == "argument")
        {
            parse_complex_push (to_string (number), "2");
        }

        if (segment == "this")
        {
            parse_complex_push (to_string (number), "3");
        }
        
        if (segment == "that")
        {
            parse_complex_push (to_string (number), "4");
        }
    }

    if (command == "pop")
    {
        if (segment == "pointer")
        {
            parse_simple_pop (to_string (number + 3) ) ; // refers to this and that which are @3 & @4
        }

        if (segment == "temp")
        {
            parse_simple_pop (to_string (number + 5) ) ; // refers to the cells between 5 and 12
        }

        if (segment == "static")
        {
            parse_simple_pop (to_string (number + 16) ) ; // refers to the cells between 16 &255
        }

        if (segment == "local")
        {
            parse_complex_pop (to_string(number), "1"); // lcl is found at @1
        }
        
        if (segment == "argument")
        {
            parse_complex_pop (to_string(number), "2"); // arg is found at @2
        }

        if (segment == "this")
        {
            parse_complex_pop (to_string(number), "3"); // this is found at @3
        }

        if (segment == "that")
        {
            parse_complex_pop (to_string(number), "4"); // that is found at @4
        }
    }

    // tell the output system that we have just finished trying to implement a VM command
    end_of_vm_command() ;
}

// main program
int main(int argc,char **argv)
{
    // config_errors(iob_buffer) ;
    // parse abstract syntax tree and pass to the translator
    translate_vm_class(ast_parse_xml()) ;

    // flush output and errors
    print_output() ;
    print_errors() ;
}