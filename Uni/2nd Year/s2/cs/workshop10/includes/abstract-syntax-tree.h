#ifndef WORKSHOP_AST_H
#define WORKSHOP_AST_H

#include <string>
#include <vector>
#include "tokeniser.h"

// Abstract Syntax Tree
// All AST nodes have a private implementation
// - the AST values passed around are indexes into a private data structure
// - vector like nodes, AST nodes are immutable
// - every AST node has a 'create' function, eg 'create_while(_condition,_statements)'
// - every field of an AST node has a 'get' function, eg, 'get_while_statements(_while_node)'
// - vector like AST nodes have a 'size_of' function, eg 'size_of_statements(_statements_node);'
// - vector like AST nodes have a 'get' function, eg 'get_statements(_statements_node,2);'

// all errors encountered by AST functions will result in calls to fatal_error() defined in iobuffer.h
// an additional error message will be printed to standard error
// the running program will terminate with an exit status of -1
// errors include
// - passing an ast value that does not identify an AST node
// - passing an ast value that refers to an inappropriate kind of AST node
// - tree traversal errors when print a tree as XML
// - parsing errors when constructing an XML tree

// An Abstract Sytax Tree is language specific and has its own private namespace, eg Workshop_Compiler

// Workshop example language syntax - the grammar we are recognising
//
// TERM: DEFINITION
// program:      declarations statement
// declarations: ('var' variable ';')*
// statement:    while |
//               if |
//               let |
//               '{' statements '}'
// while:        'while' '(' condition ')' statement
// if:           'if' '(' condition ')' statement ('else' statement)?
// let:          'let' identifier '=' expression ';'
// statements:   statement*
// expression:   term (op term)?
// condition:    term relop term
// term:         variable | int
// variable:     identifier
//
// TOKEN: DEFINITION
// identifier: ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
// int:        ('0'-'9')('0'-'9')*
// relop:      '<' | '<=' | '==' | '!=' | '>' | >='
// op:         '+' | '-' | '*' | '/'

// Workshop Compiler
namespace Workshop_Compiler
{
    // we use integers to identify AST nodes in the public interface
    typedef int ast ;

    // shorthand to avoid std::
    using namespace std ;

    // enumeration for AST node classes
    // ast_alpha & ast_omega describe the maximum range of ast_kind values
    // ast_oops is for error reporting
    enum ast_kind
    {
        ast_alpha = 3000,       // keep enum ranges unique

                                // Groupings only - there are no nodes of these kinds
                                // These are used to describe a group of nodes that are used instead
        ast_statement,          // statement - groups let, if, if_else, while, statements
        ast_expression,         // expression - groups int, variable and infix_op
        ast_term,               // term - groups int and variable

                                // Workshop example language - one node kind per grammar rule
        ast_program,            // program

        ast_declarations,       // variable declarations
        ast_variable,           // variable - identifier with segment, offset and type information
                                // assume all variables are of type "int" and are in segment "local"

        ast_statements,         // statements
        ast_let,                // let statement - variable assignment
        ast_if,                 // if then statement
        ast_if_else,            // if then else statement
        ast_while,              // while statement

        ast_int,                // integer constant
        ast_infix_op,           // infix op and operands
        ast_condition,          // relational op and operands

        ast_oops,               // for error reporting
        ast_omega               // largest ast_kind value
    };


    /***** AST Functions *****
      * IMPORTANT:
      * all errors encountered by these functions will result in calls to fatal_error()
      * an additional error message will be printed to standard error
      * the running program will terminate with an exit status of -1
    */

    //***** AST node kind functions *****//

    extern string   ast_kind_to_string(ast_kind k) ;        // returns string representation of ast kind k
    extern ast_kind ast_node_kind(ast t) ;                  // what kind of ast node is t?

                                                            // the next two functions recognise grouped kinds
    extern bool     ast_have_kind(ast t,ast_kind k) ;       // can node t be safely used as a node of kind k?
    extern void     ast_mustbe_kind(ast t,ast_kind k) ;     // call fatal_error() if node t cannot be safely used as a node of kind k

    //***** XML printing / parsing functions for Workshop AST nodes *****//

    // XML token kinds
    enum xml_token_kind
    {
        xml_opentag,    // an open tag, tag name is in spelling
        xml_closetag,   // a close tag, tag name is in spelling
        xml_text,       // a #text node, text is in spelling
        xml_eoi         // end of xml input or error?
    };

    // an XML token
    struct xml_token
    {
        xml_token_kind kind ;
        string spelling ;
    } ;

    // print tree t as XML, an indent_by > 0 causes pretty printing
    extern void ast_print_as_xml(ast t,int indent_by) ;

    // construct a Workshop_Compiler abstract syntax tree by parsing XML from standard input,
    // any pretty printed indents are ignored,
    // the result will be either an ast_tokens node or an ast_program node
    extern ast ast_parse_xml() ;

    //***** AST nodes to represent Workshop example language programs *****//
    // program ::= declarations statement
    // ast_program - a workshop example language program
    // declarations must be an ast_declarations node
    // statement must be an ast_statement node
    extern ast create_program(ast declarations,ast statement) ;
    extern ast get_program_declarations(ast program) ;
    extern ast get_program_statement(ast program) ;


    // declarations ::= ('var' variable ';')*
    // ast_declarations - a vector of ast_variable nodes
    // variables must be a vector of ast_variable nodes
    extern ast create_declarations(vector<ast> variables) ;
    extern int size_of_declarations(ast t) ;
    extern ast get_declarations(ast t,int i) ;


    // ast_variable node - an identifier with segment, offset and type informationode
    // it records the variable's name, it's segment name ("local"), it's segment offset and it's type ("int")
    // the first variable has offset 0, the second offset 1 and so on
    extern ast create_variable(string name,string segment,int offset,string type) ;
    extern string get_variable_segment(ast variable) ;
    extern string get_variable_name(ast variable) ;
    extern int get_variable_offset(ast variable) ;
    extern string get_variable_type(ast variable) ;


    // statements ::= statement*
    // ast_statements node - a statement sequence
    // statements must be a vector of ast_statement nodes
    // an ast_statement node is one of ast_let, ast_if, ast_if_else, ast_while, and ast_statements
    extern ast create_statements(vector<ast> statements) ;
    extern int size_of_statements(ast statements) ;
    extern ast get_statements(ast statements,int index) ;


    // let ::= 'let' variable '=' expression ';'
    // ast_let node - a variable assignment statement
    // variable must be an astiable_var node
    // expression must be an ast_expression node
    // an ast_expression node is one of ast_int, ast_variable or ast_infix_op
    extern ast create_let(ast variable,ast expression) ;
    extern ast get_let_variable(ast _let) ;
    extern ast get_let_expression(ast _let) ;


    // if ::= 'if' '(' condition ')' '{' statements '}'
    // ast_if node - an if then statement
    // condition must be an ast_condition node
    // if_true must be an ast_statements node
    extern ast create_if(ast condition,ast if_true) ;
    extern ast get_if_condition(ast _if) ;                  // also accepts an ast_if_else node
    extern ast get_if_if_true(ast _if) ;                    // also accepts an ast_if_else node


    // if ::= 'if' '(' condition ')' '{' statements '}' 'else' '{' statements '}'
    // ast_if_else node - an if then else statement
    // condition must be an ast_condition node
    // if_true must be an ast_statements node
    // if_false must be an ast_statements node
    extern ast create_if_else(ast condition,ast if_true,ast if_false) ;
    extern ast get_if_else_condition(ast _if) ;             // also accepts an ast_if node
    extern ast get_if_else_if_true(ast _if) ;               // also accepts an ast_if node
    extern ast get_if_else_if_false(ast _if) ;


    // while ::= 'while' '(' condition ')' '{' statements '}'
    // ast_while node - a while loop
    // condition must be an ast_condition node
    // body must be an ast_statements node
    extern ast create_while(ast condition,ast body) ;
    extern ast get_while_condition(ast _while) ;
    extern ast get_while_body(ast _while) ;


    // int ::= '0' to '32767'
    // ast_int node - an integer constant
    // _constant must be in the range 0 up to 32,767
    extern ast create_int(int _constant) ;
    extern int get_int_constant(ast _int) ;

    // ast_infix_op node - an infix op expression, ie lhs op rhs
    // lhs must be an ast_term node
    // op must be an infix op, ie '+', '-', '*' or '/'
    // rhs must be an ast_term node
    // an ast_term node is one of ast_int or ast_variable
    extern ast create_infix_op(ast lhs,string op,ast rhs) ;
    extern ast get_infix_op_lhs(ast infix_op) ;
    extern string get_infix_op_op(ast infix_op) ;
    extern ast get_infix_op_rhs(ast infix_op) ;

    // ast_condition node - a condition, ie lhs relop rhs
    // lhs must be an ast_term node
    // relop must be an infix op, ie '<', '<=', '==', '!=', '>' or '>='
    // rhs must be an ast_term node
    extern ast create_condition(ast lhs,string relop,ast rhs) ;
    extern ast get_condition_lhs(ast condition) ;
    extern string get_condition_relop(ast condition) ;
    extern ast get_condition_rhs(ast condition) ;

}

#endif //WORKSHOP_AST_H
