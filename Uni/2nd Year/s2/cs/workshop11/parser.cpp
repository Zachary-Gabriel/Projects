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

// program: declarations statement eoi
static ast parseProgram()
{
    push_error_context("parseProgram()") ;

    ast decls = parseDeclarations() ;
    ast stat = parseStatement() ;
    mustbe(tk_eoi) ;

    pop_error_context() ;

    return create_program(decls,stat) ;
}

// declarations: ('var' identifier ';')*
static ast parseDeclarations()
{
    push_error_context("parseDeclarations()") ;

    vector<ast> decls ;

    while ( have(tk_var) )
    {
        mustbe(tk_var) ;
        decls.push_back(declare_variable(mustbe(tk_identifier))) ;
        mustbe(tk_semi) ;
    }

    pop_error_context() ;
    return create_declarations(decls) ;
}

// statement:  whileStatement | ifStatement | letStatement | '{' statementSequence '}'
static ast parseStatement()
{
    switch(token_kind())
    {
    case tk_while:  return parseWhileStatement() ;
    case tk_if:     return parseIfStatement() ;
    case tk_let:    return parseLetStatement() ;
    case tk_lcb:    return parseStatementSequence() ;
    default:        mustbe(tk_statement) ;
    }

    // this should never occur, but just in case we will treat getting here as fatal!
    fatal_error(0,"parseStatement() found an unexpected statement kind") ;

    // this is just so that the compiler does not complain about no returned value
    return -1 ;
}

// whileStatement: 'while' '(' condition ')' statement
static ast parseWhileStatement()
{
    push_error_context("parseWhileStatement()") ;

    mustbe(tk_while) ;
    mustbe(tk_lrb) ;
    ast cond = parseCondition() ;
    mustbe(tk_rrb) ;
    ast stat = parseStatement() ;
    pop_error_context() ;

    return create_while(cond,stat) ;
}

// ifStatement: 'if' '(' condition ')' statement ('else' statement)?
static ast parseIfStatement()
{
    push_error_context("parseIfStatement()") ;

    mustbe(tk_if) ;
    mustbe(tk_lrb) ;
    ast cond = parseCondition() ;
    mustbe(tk_rrb) ;
    ast then_stat = parseStatement() ;

    if ( have(tk_else) )
    {
        mustbe(tk_else) ;
        ast else_stat = parseStatement() ;

        pop_error_context() ;
        return create_if_else(cond,then_stat,else_stat) ;
    } else
    {
        pop_error_context() ;
        return create_if(cond,then_stat) ;
    }
}

// letStatement: 'let' identifier '=' expression ';'
static ast parseLetStatement()
{
    push_error_context("parseLetStatement()") ;

    mustbe(tk_let) ;
    ast id = lookup_variable(mustbe(tk_identifier)) ;
    mustbe(tk_assign) ;
    ast expr = parseExpression() ;
    mustbe(tk_semi) ;
    pop_error_context() ;

    return create_let(id,expr) ;
}

// statementSequence: (statement)*
static ast parseStatementSequence()
{
    push_error_context("parseStatementSequence()") ;

    vector<ast> seq ;

    mustbe(tk_lcb) ;
    while ( have(tk_statement) )
    {
        seq.push_back(parseStatement()) ;
    }
    mustbe(tk_rcb) ;

    pop_error_context() ;
    return create_statements(seq) ;
}

// expression: term (op term)?
static ast parseExpression()
{
    push_error_context("parseExpression()") ;

    ast lhs = parseTerm() ;
    if ( have(tk_infix_op) )
    {
        Token op = mustbe(tk_infix_op) ;
        ast rhs = parseTerm() ;

        pop_error_context() ;
        return create_infix_op(lhs,token_spelling(op),rhs) ;
    }

    pop_error_context() ;
    return lhs ;
}

// condition: term relop term
static ast parseCondition()
{
    push_error_context("parseCondition()") ;

    ast lhs = parseTerm() ;
    Token op = mustbe(tk_relop) ;
    ast rhs = parseTerm() ;

    pop_error_context() ;
    return create_condition(lhs,token_spelling(op),rhs) ;
}

// term: identifier | integer
static ast parseTerm()
{
    push_error_context("parseTerm()") ;

    ast term ;
    switch(token_kind())
    {
    case tk_identifier:
        term = lookup_variable(mustbe(tk_identifier)) ;
        pop_error_context() ;
        return term ;

    case tk_integer:
        term = create_int(token_ivalue(mustbe(tk_integer))) ;
        pop_error_context() ;
        return term ;

    default:
        mustbe(tk_term) ;
    }

    // this should never occur, but just in case we will treat getting here as fatal!
    fatal_error(0,"parseTerm() found an unexpected term kind") ;

    // this is just so that the compiler does not complain about no returned value
    return -1 ;
}

// call parseProgram from here
ast parser_xml()
{
    // read first token to initialise tokeniser
    next_token() ;

    // create a new symbol table
    symbol_table = create_ints() ;

    // parse a Program to get a new document tree
    ast root = parseProgram() ;

    // return root of document tree we parsed
    return root ;
}

// main program for workshop 11 parser to XML
int main(int argc,char **argv)
{
    config_output(iob_immediate) ;
    ast_print_as_xml(parser_xml(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

