// convert Hack VM Code into a document tree
#include "iobuffer.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

// to make out programs a bit neater
using namespace std ;
using namespace CS_IO_Buffers ;

using namespace Hack_Virtual_Machine ;

// grammar to be parsed:
// vm_class ::= command* eoi
// command ::= vm_operator | vm_jump | vm_func | vm_stack
// vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
// vm_jump ::= jump label
// vm_func ::= func label number
// vm_stack ::= stack segment number

// forward declare parsing functions - one per rule
ast parse_class() ;
ast parse_command() ;
ast parse_op() ;
ast parse_jump() ;
ast parse_func() ;
ast parse_stack() ;

// Note: always read one token after the one recognised

// vm_class ::= command* eoi
// parse a vm class and return a tree node to represent it
// when called, the current token should be the first token of the class
ast parse_class()
{
    vector<ast> commands ;

    // we stop when the next token does not start a vm command
    while ( have(tk_vm_command) )
    {

        // we parse the command and get back the command represented as a tree node 
        // append the new tree node to the commands vector
        commands.push_back(parse_command()) ;

    }

    // if we do not see the start of a new vm command we must be at the end of input
    mustbe(tk_eoi) ;

    // return a new tree node for a class initialised using
    // the vector of parsed commands
    return create_vm_class(commands) ;
}

// command ::= vm_operator | vm_jump | vm_func | vm_stack
// parse a vm command and return a tree node to represent it
// when called, the current token should be the first token of the command
ast parse_command()
{
    // inspect the token to see what it might start
    if ( have(tk_vm_operator) ) return parse_op() ;
    if ( have(tk_vm_jump) )     return parse_jump() ;
    if ( have(tk_vm_func) )     return parse_func() ;
    if ( have(tk_vm_stack) )    return parse_stack() ;

    // if none of the above match we have an error
    // this mustbe will raise an error because we have covered all possibilites above
    mustbe(tk_vm_command) ;

    // this will never be executed but it included so the compiler can 
    // see a return of the correct type
    return -1 ;
}

// vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
// parse a vm operator command and return a tree node to represent it
// when called, the current token should be the first token of the command
ast parse_op()
{
    /*****************   MODIFY THE FOLLOWING CODE  ******************/

    // Modify this code to check that the next part of the input is a legal vm_operator command and initiliase variable op appropriately


    // the operator - it must be one of the tokens in the group tk_vm_operator
    string op = "" ;


    /*****************   XXXXXX   TO HERE   XXXXXX  ******************/

    // this creates a new tree node to represent the operator initialised by op
    return create_vm_operator(op) ;
}

// vm_jump ::= jump label
// parse a vm jump command and return a tree node to represent it
// when called, the current token should be the first token of the command
ast parse_jump()
{
    /*****************   MODIFY THE FOLLOWING CODE  ******************/

    // Modify this code to check that the next part of the input is a legal vm_jump command and initiliase variables command and label appropriately


    // jump command - goto, if-goto or label - it must be one of the tokens in the group tk_vm_jump
    string command = "" ;

    // label name - it must be a tk_a_label token
    string label = "" ;


    /*****************   XXXXXX   TO HERE   XXXXXX  ******************/

    // this creates a new tree node to represent the jump initialised by command and label
    return create_vm_jump(command,label) ;
}

// vm_func ::= func label number
// parse a vm func command and return a tree node to represent it
// when called, the current token should be the first token of the command
ast parse_func()
{
    /*****************   MODIFY THE FOLLOWING CODE  ******************/

    // Modify this code to check that the next part of the input is a legal vm_func command and initiliase variables command, label and number appropriately


    // function command - call or function - it must be one of the tokens in the group tk_vm_func
    string command = "" ;

    // function name - a label - it must be a tk_a_label token
    string label = "" ;

    // number of args or locals - it must be a tk_a_number token
    int number = 0 ;


    /*****************   XXXXXX   TO HERE   XXXXXX  ******************/

    // this creates a new tree node to represent the func command initialised by command, label and number
    return create_vm_function(command,label,number) ;
}

// vm_stack ::= stack segment number
// parse a vm stack command and return a tree node to represent it
// when called, the current token should be the first token of the command
ast parse_stack()
{
    /*****************   MODIFY THE FOLLOWING CODE  ******************/

    // Modify this code to check that the next part of the input is a legal vm_stack command and initiliase variables stack, segment and number appropriately


    // stack command - push or pop - it must be one of the tokens in the group tk_vm_stack
    string stack = "" ;

    // segment name - it must be one of the tokens in the group tk_vm_segment
    string segment = "" ;

    // offset - it must be a tk_a_number token
    int number = 0 ;


    /*****************   XXXXXX   TO HERE   XXXXXX  ******************/

    // this creates a new tree node to represent the stack command initialised by stack, segment and number
    return create_vm_stack(stack,segment,number) ;
}

// main program
int main(int argc,char **argv)
{
    // initialise the tokeniser by reading the first token
    next_token() ;

    // parse a class and print the abstract syntax tree as XML
    ast_print_as_xml(parse_class(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

