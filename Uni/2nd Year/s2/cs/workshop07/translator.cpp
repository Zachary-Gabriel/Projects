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



////////////////////////////////////////////////////////////////

/************   PUT YOUR HELPER FUNCTIONS HERE   **************/



/************      END OF HELPER FUNCTIONS       **************/

///////////////////////////////////////////////////////////////



// the function translate_vm_class() will be called by the main program
// its is passed the abstract syntax tree constructed by the parser
// it walks the abstract syntax tree and produces the equivalent VM code as output
static void translate_vm_class(ast root)
{
    // we must be passed a "vm_class" node containing VM command nodes
    ast_mustbe_kind(root,ast_vm_class) ;

    // tell the output system we are starting to translate VM commands for a Jack class
    start_of_vm_class() ;

    // translate the commands in the order they were added to the class
    int ncommands = size_of_vm_class(root) ;
    for ( int i = 0 ; i < ncommands ; i++ )
    {
        translate_vm_command(get_vm_class(root,i)) ;
    }

    // tell the output system we have just finished translating VM commands for a Jack class
    end_of_vm_class() ;

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

    /************   ADD CODE BETWEEN HERE   **************/

    // use the output_assembler() function to implement this VM command in Hack Assembler
    // careful use of helper functions you can define above will keep your code simple
    // ...


    /************         AND HERE          **************/

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

    /************   ADD CODE BETWEEN HERE   **************/

    // use the output_assembler() function to implement this VM command in Hack Assembler
    // careful use of helper functions you can define above will keep your code simple
    // ...


    /************         AND HERE          **************/

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

    // tell the output system what kind of VM command we are now trying to implement
    start_of_vm_func_command(command,label,number) ;

    /************   ADD CODE BETWEEN HERE   **************/

    // use the output_assembler() function to implement this VM command in Hack Assembler
    // careful use of helper functions you can define above will keep your code simple
    // ...

    if ( command == "function")
    {
        output_assembler("// "+command+" "+label+" "+to_string(number)) ;
        output_assembler("("+label+")") ;
    }

    /************         AND HERE          **************/

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

    /************   ADD CODE BETWEEN HERE   **************/

    // use the output_assembler() function to implement this VM command in Hack Assembler
    // careful use of helper functions you can define above will keep your code simple
    // ...


    /************         AND HERE          **************/

    // tell the output system that we have just finished trying to implement a VM command
    end_of_vm_command() ;
}

// main program
int main(int argc,char **argv)
{
    // parse abstract syntax tree and pass to the translator
    translate_vm_class(ast_parse_xml()) ;

    // flush output and errors
    print_output() ;
    print_errors() ;
}
