#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

using namespace std ;

using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Workshop_Compiler ;

// keep global counters so we can create unique labels in while and if statements
static int while_counter = 0 ;
static int if_counter = 0 ;

// we have a legal infix operator, translate into VM command equivalent
static string translate_op(string op)
{
    if ( op == "+" ) return "add" ;
    if ( op == "-" ) return "sub" ;
    if ( op == "*" ) return "call Math.multiply 2" ;
    if ( op == "/" ) return "call Math.divide 2" ;
    fatal_error(-1,"translate_op passed unknown op: " + op + "\n") ;
    return op ;
}

// we have a legal relational operator, translate into VM command equivalent
static string translate_relop(string op)
{
    if ( op == "<" ) return "lt" ;
    if ( op == "<=" ) return "gt\nnot" ;
    if ( op == "==" ) return "eq" ;
    if ( op == "!=" ) return "eq\nnot" ;
    if ( op == ">" ) return "gt" ;
    if ( op == ">=" ) return "lt\nnot" ;
    fatal_error(-1,"translate_relop passed unknown op: " + op + "\n") ;
    return op ;
}

// the grammar we are recognising
// TERM: DEFINITION
// program: declarations statement eoi
// declarations: ('var' identifier ';')*
// statement:  whileStatement |
//         ifStatement |
//         letStatement |
//         '{' statementSequence '}'
// whileStatement: 'while' '(' condition ')' statement
// ifStatement: 'if' '(' condition ')' statement ('else' statement)?
// letStatement: 'let' identifier '=' expression ';'
// statementSequence: (statement)*
// expression: term (op term)?
// condition: term relop term
// term: identifier | integerConstant
// TOKEN: DEFINITION
// identifier: ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
// integerConstant:    ('0'-'9')('0'-'9')*
// relop:      '<' | '<=' | '==' | '!=' | '>' | >='
// op:     '+' | '-' | '*' | '/'
// keyword:    'var' | 'while' | 'if' | 'else' | 'let'
// symbol:     '{' | '}' | '(' | ')' | ';' | '='


// since parsing is recursive, forward declare one function to parse each non-terminal:
static void parseProgram(ast) ;
static int parseDeclarations(ast) ;
static void parseStatement(ast) ;
static void parseWhileStatement(ast) ;
static void parseIfStatement(ast) ;
static void parseIfElseStatement(ast) ;
static void parseLetStatement(ast) ;
static void parseStatementSequence(ast) ;
static void parseExpression(ast) ;
static void parseCondition(ast) ;
static void parseTerm(ast) ;

// now implement the parsing functions
// ast create_program(ast declarations,ast statement)
static void parseProgram(ast n)
{
    int nlocals = parseDeclarations(get_program_declarations(n)) ;

    // if the programs starts with x variable declarations, we must start with:
    //     function Main.main x
    // nextlocal is effectively a variable count so ...
    write_to_output("function Main.main " + to_string(nlocals) + "\n") ;

    parseStatement(get_program_statement(n)) ;

    // always finish with return so the VM code is a complete void function
    write_to_output("push constant 0\n") ;
    write_to_output("return\n") ;
}

// ast create_declarations(vector<ast> variables)
static int parseDeclarations(ast n)
{
    return size_of_declarations(n) ;
}

// there are no statement nodes, only ast_while, ast_if, ast_if_else, ast_let and ast_statements nodes
static void parseStatement(ast n)
{
    switch(ast_node_kind(n))
    {
    case ast_while:
        parseWhileStatement(n) ;
        break ;
    case ast_if:
        parseIfStatement(n) ;
        break ;
    case ast_if_else:
        parseIfElseStatement(n) ;
        break ;
    case ast_let:
        parseLetStatement(n) ;
        break ;
    case ast_statements:
        parseStatementSequence(n) ;
        break ;
    default:
        fatal_error(0,"Unknown kind of statement node found") ;
        break ;
    }
}

// ast create_while(ast condition,ast body)
static void parseWhileStatement(ast n)
{
    string label = to_string(while_counter++) ;

    parseCondition(get_while_condition(n)) ;
    parseStatement(get_while_body(n)) ;
}

// ast create_if(ast condition,ast if_true)
static void parseIfStatement(ast n)
{
    string label = to_string(if_counter++) ;

    parseCondition(get_if_condition(n)) ;
    parseStatement(get_if_if_true(n)) ;
}

// ast create_if_else(ast condition,ast if_true,ast if_false)
static void parseIfElseStatement(ast n)
{
    string label = to_string(if_counter++) ;

    parseCondition(get_if_else_condition(n)) ;
    parseStatement(get_if_else_if_true(n)) ;
    parseStatement(get_if_else_if_false(n)) ;
}

// ast create_let(ast variable,ast expression)
static void parseLetStatement(ast n)
{
    ast var = get_let_variable(n) ;
    parseExpression(get_let_expression(n)) ;
}

// ast create_statements(vector<ast> statements) ;
static void parseStatementSequence(ast n)
{
    int children = size_of_statements(n) ;
    for ( int i = 0 ; i < children ; i++ )
    {
        parseStatement(get_statements(n,i)) ;
    }
}

// there are no expression nodes, only ast_infix_op, ast_variable and ast_int nodes
// ast create_infix_op(ast lhs,string op,ast rhs)
static void parseExpression(ast n)
{
    if ( ast_have_kind(n,ast_infix_op) )
    {
        parseTerm(get_infix_op_lhs(n)) ;
        parseTerm(get_infix_op_rhs(n)) ;
        string op = get_infix_op_op(n) ;
    }
    else
    {
        parseTerm(n) ;
    }
}

// ast create_condition(ast lhs,string relop,ast rhs)
static void parseCondition(ast n)
{
    parseTerm(get_condition_lhs(n)) ;
    parseTerm(get_condition_rhs(n)) ;
    string relop = get_condition_relop(n) ;
}

// there are no term nodes, only ast_variable and ast_int nodes
// ast create_variable(string name,string segment,int offset,string type)
static void parseTerm(ast n)
{
    switch(ast_node_kind(n))
    {
    case ast_variable:
        break ;
    case ast_int:
        break ;
    default:
        fatal_error(0,"Unknown kind of term node found") ;
        break ;
    }
}

// main program for workshop 11 XML to VM code translator
int main(int argc,char **argv)
{
    parseProgram(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}
