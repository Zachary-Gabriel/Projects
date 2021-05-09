#include "iobuffer.h"
#include "symbols.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

using namespace std ;

using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Workshop_Compiler ;

// symbol table management so we can lookup declared variables
static symbols symbol_table = 0 ;

// a global to record the next free position in the local segment
static int nextlocal = 0 ;

// this function adds an identifier to the symbol table
// it returns a tree node representing the variable
static ast declare_variable(Token identifier)
{
    string varname = token_spelling(identifier) ;

    if ( !insert_ints(symbol_table,varname,nextlocal) ) // it is an error to declare something twice
    {
        fatal_error(0,"\n" + token_context(identifier) + "Variable:  " + varname +  " has already been declared") ;
    }

    return create_variable(varname,"local",nextlocal++,"int") ;
}

// lookup the identifier, it is an error if it was not previously declared
// it returns a tree node representing the variable
static ast lookup_variable(Token identifier)
{
    string varname = token_spelling(identifier) ;
    int offset = lookup_ints(symbol_table,varname) ;

    if ( offset == -1 )
    {
        fatal_error(0,"\n" + token_context(identifier) + "Found undeclared variable:  " + varname) ;
    }

    return create_variable(varname,"local",offset,"int") ;
}

// the grammar we are recognising
// TERM: DEFINITION
// program: declarations statement eoi
// declarations: ('var' identifier ';')*
// statement:  whileStatement | ifStatement | letStatement | statementSequence
// whileStatement: 'while' '(' condition ')' statement
// ifStatement: 'if' '(' condition ')' statement ('else' statement)?
// letStatement: 'let' identifier '=' expression ';'
// statementSequence: '{' (statement)* '}'
// expression: term (op term)?
// condition: term relop term
// term: identifier | integer
//
// TOKEN: DEFINITION
// identifier: ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
// integer:    ('0'-'9')('0'-'9')*
// relop:      '<' | '<=' | '==' | '!=' | '>' | >='
// op:         '+' | '-' | '*' | '/'
// keyword:    'var' | 'while' | 'if' | 'else' | 'let'
// symbol:     '{' | '}' | '(' | ')' | ';' | '='

// since parsing is recursive, forward declare one function to parse each non-terminal
// the functions are declared static because thye should not be called form outside this file
// in this parser every parse function returns an abstract syntax tree representing what it has parsed
static ast parseProgram() ;
static ast parseDeclarations() ;
static ast parseStatement() ;
static ast parseWhileStatement() ;
static ast parseIfStatement() ;
static ast parseLetStatement() ;
static ast parseStatementSequence() ;
static ast parseExpression() ;
static ast parseCondition() ;
static ast parseTerm() ;

// now implement the parsing functions
// note: we have added push/pop_error_context() calls so that 
//       you can see part of the call chain when an error occurs
// note: we have chosen not to create separate nodes for statement and term
//       where one of these occur we simply return a node representing
//       the particular kind of statement or term found





// ***** COMPLETE THE CODE BELOW TO COMPLETE THE PARSER *****

// In each parse*() function write code to correctly initialise
// the variables passed to the tree node create*() function




// program: declarations statement eoi
static ast parseProgram()
{
    push_error_context("parseProgram()") ;

    ast decls ;         // the declarations
    ast stat ;          // the single statement that is the program
    
    decls = parseDeclarations() ;
    stat = parseStatement() ;
    
    mustbe(tk_eoi) ;    // after parsing a class the next token must be the end of input marker

    pop_error_context() ;

    return create_program(decls,stat) ;
}

// declarations: ('var' identifier ';')*
static ast parseDeclarations()
{
    push_error_context("parseDeclarations()") ;

    vector<ast> decls ; // we use a vector collect up all the declarations for later

    while (have(tk_var))
    {
        ast decl ;      // the result of passing the identifier found to declare_variable()
        Token id = mustbe(tk_identifier) ;
        mustbe(tk_semi) ;
        decls.push_back(decl) ;
    }

    pop_error_context() ;
    return create_declarations(decls) ;
}

// statement:  whileStatement | ifStatement | letStatement | statementSequence
static ast parseStatement()
{

    ast stat ;          // the actual whileStatement, ifStatement, letStatement of statementSequence
    if ( have(tk_while) ) return parseWhileStatement();
    if ( have(tk_if) ) return parseIfStatement();
    // we do not create any statement nodes
    return stat ;
}

// whileStatement: 'while' '(' condition ')' statement
static ast parseWhileStatement()
{
    push_error_context("parseWhileStatement()") ;

    ast cond ;          // the while statement's condition expression
    ast stat ;          // the statement to be executed while the loop condition evaluates to true

    pop_error_context() ;
    return create_while(cond,stat) ;
}

// ifStatement: 'if' '(' condition ')' statement ('else' statement)?
static ast parseIfStatement()
{
    push_error_context("parseIfStatement()") ;

    ast cond ;          // the if statement's condition expression
    ast then_stat ;     // the statement to be executed if the condition evaluates to true

    if ( !have(tk_else) )
    {
        pop_error_context() ;
        return create_if(cond,then_stat) ;
    }
    else
    {
        ast else_stat ; // the statement to be executed if the condition evaluates to false

                        // this is the return used if we found the keyword else
        pop_error_context() ;
        return create_if_else(cond,then_stat,else_stat) ;
    }
}

// letStatement: 'let' identifier '=' expression ';'
static ast parseLetStatement()
{
    push_error_context("parseLetStatement()") ;

    ast id ;            // a tree node representing the variable being assigned to
    ast expr ;          // the expression whose is being assigned to the variable

    pop_error_context() ;
    return create_let(id,expr) ;
}

// statementSequence: '{' (statement)* '}'
static ast parseStatementSequence()
{
    push_error_context("parseStatementSequence()") ;

    vector<ast> seq ;   // a vector of all parse trees returned by parsing the statements in the sequence

    // while the next token can start a statement
    {
        ast stat ;      // the next statement in the sequence

        seq.push_back(stat) ;
    }

    pop_error_context() ;
    return create_statements(seq) ;
}

// expression: term (op term)?
static ast parseExpression()
{
    push_error_context("parseExpression()") ;

    ast lhs ;           // the term on the left hand side (lhs) of the infix operator

    // if the next token is not an infix operator
    {
        pop_error_context() ;
        return lhs ;
    }
    // else
    {
        Token op ;      // the infix operator
        ast rhs ;       // the term on the right hand side (rhs) of the infix operator

        pop_error_context() ;
        return create_infix_op(lhs,token_spelling(op),rhs) ;
    }

}

// condition: term relop term
static ast parseCondition()
{
    push_error_context("parseCondition()") ;

    ast lhs ;           // the term on the left hand side (lhs) of the relational operator
    Token op ;          // the relational operator
    ast rhs ;           // the term on the right hand side (rhs) of the relational operator

    pop_error_context() ;
    return create_condition(lhs,token_spelling(op),rhs) ;
}

// term: identifier | integer
static ast parseTerm()
{
    push_error_context("parseTerm()") ;

    ast term ;          // the identifier or integer that is the term

    pop_error_context() ;
    return term ;
}



// ***** DO NOT CHANGE CODE AFTER THIS POINT *****



// call parseProgram from here
ast parser_xml()
{
    // read first token to initialise tokeniser
    next_token() ;

    // create a new symbol table
    symbol_table = create_ints() ;

    // parse a Program to get a new document tree
    ast program = parseProgram() ;

    // return the document tree we parsed
    return program ;
}

// main program for workshop 11 parser to XML
int main(int argc,char **argv)
{
    // construct a parse tree for the program being read from standard input
    // and write it to standard output formatted as XML
    ast_print_as_xml(parser_xml(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

