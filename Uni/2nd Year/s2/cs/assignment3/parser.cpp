#include <string>
#include <vector>
#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to parse a Jack class and construct the equivalent abstract syntax tree, ast.
//        The main function will print the ast formatted as XML
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// NOTE: use the fatal function below to print fatal error messages with the current
//       input file position marked.
//
// The skeleton code has one function per non-terminal in the Jack grammar
//  - comments describing how to create relevant ast nodes prefix most functions
//  - ast nodes are immutable - you must create all the sub-trees before creating a new node
//
// The structure of the skeleton code is just a suggestion
//  - it matches the grammar but does not quite match the ast structure
//  - you will need to change the parameters / results / functions to suit your own logic
//  - you can change it as much as you like
//
// NOTE: the best place to start is to write code to parse the input without attempting to create
//       any ast nodes. If correct this parser will report errors when it finds syntax errors but
//       correct programs will fail because the tree printer will not be provided with a valid ast.
//       Once the parsing works modify your code to create the ast nodes.
//
// NOTE: the return -1 ; statements are only present so that this skeleton code will compile.
//       these statements need to be replaced by code that creates ast nodes.
//

// fatal error handling
static void fatal(string message)
{
    fatal_error(0,"***** Error:\n" + token_context() + message) ;
}

// The Jack grammar to be recognised:
// program          ::= One or more classes, each class in a separate file named <class_name>'.Jack'
// class            ::= 'class' identifier '{' class_var_decs subr_decs '}'
// class_var_decs   ::= (static_var_dec | field_var_dec)*
// static_var_dec   ::= 'static' type identifier (',' identifier)* ';'
// field_var_dec    ::= 'field' type identifier (',' identifier)* ';'
// type             ::= 'int' | 'char' | 'boolean' | identifier
// vtype            ::= 'void' | type
// subr_decs        ::= (constructor | function | method)*
// constructor      ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// function         ::= 'function' vtype identifier '(' param_list ')' subr_body
// method           ::= 'method' vtype identifier '(' param_list ')' subr_body
// param_list       ::= ((type identifier) (',' type identifier)*)?
// subr_body        ::= '{' var_decs statements '}'
// var_decs         ::= var_dec*
// var_dec          ::= 'var' type identifier (',' identifier)* ';'

// statements       ::= statement*
// statement        ::= let | if | while | do | return
// let              ::= 'let' identifier index? '=' expr ';'
// if               ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// while            ::= 'while' '(' expr ')' '{' statements '}'
// do               ::= 'do' identifier (id_call | call) ';'
// return           ::= 'return' expr? ';'

// expr             ::= term (infix_op term)*
// term             ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// var_term         ::= identifier (index | id_call | call)?
// index            ::= '[' expr ']'
// id_call          ::= '.' identifier call
// call             ::= '(' expr_list ')'
// expr_list        ::= (expr (',' expr)*)?
// infix_op         ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unary_op         ::= '-' | '~'


// forward declarations of one function per non-terminal rule in the Jack grammar - except for program
// NOTE: parse_type(), parse_vtype(), parse_unary_op() all return a Token not an ast
// NOTE: parse_static_var_dec(), parse_field_var_dec(), parse_var_dec() all return vector<ast> not an ast
symbols tempLclVars = create_variables ();
symbols tempArgVars = create_variables ();
symbols fldVars = create_variables () ;
symbols stcVars = create_variables () ;

string className = "";
symbols functionNames = create_ints ();
symbols methodNames = create_ints ();
symbols constructors = create_ints ();

static int fieldspace = 0;
static int staticspace = 0;
static int localspace = 0;

int var_location = 0;
bool parser_is_constructor = false;
bool parser_is_void = false;
bool must_return = false;
bool parser_is_function = false;

ast parse_class() ;
ast parse_class_var_decs() ;
vector<ast> parse_static_var_dec() ;
vector<ast> parse_field_var_dec() ;
Token parse_type() ;
Token parse_vtype() ;
ast parse_subr_decs() ;
ast parse_constructor() ;
ast parse_function() ;
ast parse_method() ;
ast parse_param_list() ;
ast parse_subr_body() ;
ast parse_var_decs() ;
vector<ast> parse_var_dec() ;

ast parse_statements() ;
ast parse_statement() ;
ast parse_let() ;
ast parse_if() ;
ast parse_while() ;
ast parse_do() ;
ast parse_return() ;

ast parse_expr() ;
ast parse_term() ;
ast parse_var_term() ;
ast parse_index() ;
ast parse_id_call() ;
ast parse_call() ;
ast parse_expr_list() ;
ast parse_infix_op() ;
Token parse_unary_op() ;

// class ::= 'class' identifier '{' class_var_decs subr_decs '}'
// create_class(myclassname,class_var_decs,class_subrs)
ast parse_class ()
{
    push_error_context ("parse_class()") ;
    
    mustbe (tk_class) ;
    string myclassname = token_spelling () ;
    className = myclassname ;
    mustbe (tk_identifier) ;
    mustbe (tk_lcb) ;

    // nesting everything inside the class
    ast class_var_decs = parse_class_var_decs () ;
    ast class_subrs = parse_subr_decs () ;
    mustbe (tk_rcb) ;
    
    pop_error_context () ;
    return create_class (myclassname, class_var_decs, class_subrs) ;    
}

// class_var_decs ::= (static_var_dec | field_var_dec)*
// returns ast_class_var_decs: create_class_var_decs(vector<ast> decs)
// create_class_var_decs must be passed a vector of ast_var_dec
//
ast parse_class_var_decs ()
{
    vector<ast> decs ;
    push_error_context ("parse_class_var_decs()") ;
    while ( have (tk_static) || have (tk_field) )
    {
        if ( have (tk_static) ) 
        {
            vector<ast> temp = parse_static_var_dec() ;
            decs.insert( decs.end(), temp.begin(), temp.end() );
        }
        else
        {
            vector<ast> temp = parse_field_var_dec() ;
            decs.insert( decs.end(), temp.begin(), temp.end() );
        }
    }

    pop_error_context () ;
    return create_class_var_decs (decs) ;
}

// static_var_dec ::= 'static' type identifier (',' identifier)* ';'
// returns vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
vector<ast> parse_static_var_dec ()
{
    vector<ast> decs ;
    push_error_context ("parse_class()") ;
    
    // for static
    mustbe (tk_static) ;
    
    // for type
    string type = token_spelling () ;
    mustbe (tk_type) ;

    // for name
    string name = token_spelling () ;
    mustbe (tk_identifier) ;

    decs.push_back ( create_var_dec (name, "static", staticspace, type) ) ;

    st_variable tableVar (name, type, "static", staticspace) ;
    st_variable temp = lookup_variables (stcVars, name) ;
    if ( temp.offset == -1 )
    {
        temp = lookup_variables (fldVars, name) ;
        if ( temp.offset == -1 )
        {
            insert_variables (stcVars, name, tableVar) ;
        }
        else
        {
            fatal (name + " already declared in this scope");
        }
    }
    else
    {
        fatal (name + " already declared in this scope");
    }
    staticspace++;

    while ( have (tk_comma) )
    {
        mustbe (tk_comma) ;
        string name = token_spelling () ;
        mustbe (tk_identifier) ;

        decs.push_back ( create_var_dec (name, "static", staticspace, type) ) ;

        st_variable tableVar (name, type, "static", staticspace) ;
        st_variable temp = lookup_variables (stcVars, name) ;
        if ( temp.offset == -1 )
        {
            temp = lookup_variables (fldVars, name) ;
            if ( temp.offset == -1 )
            {
                insert_variables (stcVars, name, tableVar) ;
            }
            else
            {
                fatal (name + " already declared in this scope");
            }
        }
        else
        {
            fatal (name + " already declared in this scope");
        }
        staticspace++;

    }
    mustbe (tk_semi) ;

    pop_error_context() ;
    return decs ;
}

// field_var_dec ::= 'field' type identifier (',' identifier)* ';'
// returns vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
vector<ast> parse_field_var_dec ()
{
    vector<ast> decs ;
    push_error_context ("parse_class()") ;
    
    
    // for field
    mustbe (tk_field) ;
    
    // for type
    string type = token_spelling () ;
    mustbe (tk_type) ;

    // for name
    string name = token_spelling () ;
    mustbe (tk_identifier) ;
    decs.push_back ( create_var_dec (name, "this", fieldspace, type) ) ;

    st_variable tableVar (name, type, "this", fieldspace) ;
    st_variable temp = lookup_variables (fldVars, name) ;
    if ( temp.offset == -1 )
    {
        temp = lookup_variables (stcVars, name) ;
        if ( temp.offset == -1 )
        {
            insert_variables (stcVars, name, tableVar) ;
        }
        else
        {
            fatal (name + " already declared in this scope");
        }
    }
    else
    {
        fatal (name + " already declared in this scope");
    }
    ++fieldspace;

    while ( have (tk_comma) )
    {
        mustbe (tk_comma) ;
        name = token_spelling () ;
        mustbe (tk_identifier) ;

        decs.push_back ( create_var_dec (name, "this", fieldspace, type) ) ;
        st_variable tableVar (name, type, "this", fieldspace) ;
        st_variable temp = lookup_variables (fldVars, name) ;
        if ( temp.offset == -1 )
        {
            temp = lookup_variables (stcVars, name) ;
            if ( temp.offset == -1 )
            {
                insert_variables (stcVars, name, tableVar) ;
            }
            else
            {
                fatal (name + " already declared in this scope");
            }
        }
        else
        {
            fatal (name + " already declared in this scope");
        }
        ++fieldspace;

    }
    mustbe (tk_semi) ;

    pop_error_context() ;
    return decs ;
}

// type ::= 'int' | 'char' | 'boolean' | identifier
// returns the Token for the type
Token parse_type ()
{
    push_error_context ("parse_type()") ;
    Token current = mustbe(tk_type) ;

    if ( have (current, tk_identifier) )
    {
        pop_error_context() ;
        return current ;
    }
    if ( have (current, tk_int) )
    {
        pop_error_context() ;
        return current ;
    }
    if ( have (current, tk_boolean) )
    {
        pop_error_context() ;
        return current ;
    }
    if ( have (current, tk_char) )
    {
        pop_error_context() ;
        return current ;
    }

    pop_error_context() ;
    return current ;
}

// vtype ::= 'void' | type
// returns the Token for the type
Token parse_vtype ()
{
    push_error_context ("parse_vtype()") ;
    
    if ( have (tk_void) ) 
    {
        parser_is_void = true;
        Token temp = mustbe (tk_void) ;
        pop_error_context() ;
        return temp ;
    }
    else
    {
        parser_is_void = false;
        pop_error_context() ;
        return parse_type () ;
    }
}

// subr_decs ::= (constructor | function | method)*
// returns ast_subr_decs: create_subr_decs(vector<ast> subrs)
// create_subr_decs must be passed an vector of ast_subr
//
// ast_subr: create_subr(ast subr)
// create_subr must be passed one of: ast_constructor, ast_function or ast_method
//
ast parse_subr_decs ()
{
    push_error_context ("parse_subr_decs()") ;
    
    vector<ast> subrs ;
    while ( have (tk_subroutine) )
    {
        if ( have (tk_constructor) ) 
        {
            ast cnstrct = parse_constructor () ;
            ast subr = create_subr ( cnstrct ) ;
            subrs.push_back ( subr ) ;
        }
        else if ( have (tk_function) ) 
        {
            ast funct = parse_function () ;
            ast subr = create_subr ( funct ) ;
            subrs.push_back ( subr ) ;
        }
        else if ( have (tk_method) ) 
        {
            ast meth = parse_method () ;
            ast subr = create_subr ( meth ) ;
            subrs.push_back ( subr ) ;
        }
    }

    pop_error_context () ;
    return create_subr_decs (subrs) ;
}

// constructor ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// returns ast_constructor: create_constructor(string vtype,string name,ast params,ast body)
// . vtype: the constructor's return type, this must be it's class name
// . name: the constructor's name within its class
// . params: ast_param_list - the constructor's parameters
// . body: ast_subr_body - the constructor's body
//
ast parse_constructor ()
{
    mustbe (tk_constructor);
    push_error_context ("parse_constructor()") ;
    
    parser_is_constructor = true;
    string vtype = token_spelling () ;
    mustbe (tk_identifier) ;
    if (vtype != className)
    {
        fatal ("incoherent return types") ;
    }
    string name = token_spelling () ;
    mustbe (tk_identifier) ;
    
    mustbe (tk_lrb) ;
    ast paramlist = parse_param_list () ;
    mustbe (tk_rrb) ;
    ast subr_body = parse_subr_body () ;

    insert_ints (constructors, name, var_location) ;
    ++ var_location ;

    tempArgVars = create_variables () ;
    tempLclVars = create_variables () ;
    parser_is_constructor = false;

    pop_error_context() ;
    return create_constructor (vtype, name, paramlist, subr_body) ;
}

// function ::= 'function' vtype identifier '(' param_list ')' subr_body
// returns ast_function: create_function(string vtype,string name,ast params,ast body)
// . vtype: the function's return type
// . name: the function's name within its class
// . params: ast_param_list - the function's parameters
// . body: ast_subr_body - the function's body
//
ast parse_function()
{
    mustbe (tk_function);
    push_error_context("parse_function()") ;
    parser_is_function = true ;
    Token vtypeToken = parse_vtype () ;
    string vtype = token_spelling (vtypeToken) ;

    string name = token_spelling () ;
    mustbe (tk_identifier) ;
    mustbe (tk_lrb) ;
    ast paramlist = parse_param_list () ;
    mustbe (tk_rrb) ;
    ast subr_body = parse_subr_body () ;

    insert_ints(functionNames, name, var_location) ;
    ++ var_location ;

    tempArgVars = create_variables () ;
    tempLclVars = create_variables () ;
    
    parser_is_function = false;
    parser_is_void = false;

    pop_error_context() ;
    return create_function (vtype, name, paramlist, subr_body) ;
}

// method ::= 'method' vtype identifier '(' param_list ')' subr_body
// returns ast_method: create_method(string vtype,string name,ast params,ast body)
// . vtype: the method's return type
// . name: the method;s name within its class
// . params: ast_param_list - the method's explicit parameters
// . body: ast_subr_body - the method's body
//
ast parse_method()
{
    mustbe (tk_method);
    push_error_context("parse_method()") ;

    Token vtypeToken = parse_vtype () ;
    string vtype = token_spelling (vtypeToken) ;

    string name = token_spelling () ;
    mustbe (tk_identifier) ;
    
    mustbe (tk_lrb) ;
    ast paramlist = parse_param_list () ;
    mustbe (tk_rrb) ;
    ast subr_body = parse_subr_body () ;
    
    insert_ints(methodNames, name, var_location) ;
    ++ var_location ;

    tempArgVars = create_variables () ;
    tempLclVars = create_variables () ;
    parser_is_void = false;

    pop_error_context() ;
    return create_method (vtype, name, paramlist, subr_body) ;
}

// param_list ::= ((type identifier) (',' type identifier)*)?
// returns ast_param_list: create_param_list(vector<ast> params)
// create_param_list must be passed a vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
ast parse_param_list()
{
    tempArgVars = create_variables () ;

    push_error_context("parse_param_list()") ;
    vector<ast> params;

    if ( have (tk_rrb) ) 
    {
        pop_error_context() ;
        return create_param_list ( params ) ; // for ?
    }

    int argspace = 1;
    if (parser_is_function)
    {
        argspace = 0;
    }

    string type = token_spelling () ;
    mustbe (tk_type) ;

    string name = token_spelling () ;
    mustbe (tk_identifier) ;

    ast var_dec = create_var_dec (name, "argument", argspace, type) ;
    params.push_back ( var_dec ) ;
    st_variable tableVar (name, type, "argument", argspace) ;

    st_variable temp = lookup_variables (tempArgVars, name) ;
    if ( temp.offset == -1 )
    {
        temp = lookup_variables (tempLclVars, name) ;
        if ( temp.offset == -1 )
        {
            insert_variables (tempLclVars, name, tableVar) ;
        }
        else
        {
            fatal (name + " already declared in this scope");
        }
    }
    else
    {
        fatal(name+ " was already declared in this scope");
    }
    ++argspace ;
    while ( have (tk_comma) ) // for *
    {
        mustbe (tk_comma) ;
        
        
        type = token_spelling () ;
        mustbe (tk_type) ;

        name = token_spelling () ;
        mustbe (tk_identifier) ;
        
        var_dec = create_var_dec (name, "argument", argspace, type) ;
        params.push_back ( var_dec ) ;

        st_variable tableVar (name, type, "argument", argspace) ;
        st_variable temp = lookup_variables (tempArgVars, name) ;
        if ( temp.offset == -1 )
        {
            temp = lookup_variables (tempLclVars, name) ;
            if ( temp.offset == -1 )
            {
                insert_variables (tempLclVars, name, tableVar) ;
            }
            else
            {
                fatal (name + " already declared in this scope");
            }
        }
        else
        {
            fatal(name+ " was already declared in this scope");
        }
        ++argspace ;
    }
    pop_error_context() ;
    return create_param_list ( params ) ;
}

// subr_body ::= '{' var_decs statements '}'
// returns ast_subr_body: create_subr_body(ast decs,ast body)
// create_subr_body must be passed:
// . decs: ast_var_decs - the subroutine's local variable declarations
// . body: ast_statements - the statements within the body of the subroutinue
//
ast parse_subr_body()
{
    push_error_context("parse_subr_body()") ;
    mustbe (tk_lcb) ;
    ast decs = parse_var_decs () ;
    ast body = parse_statements() ;
    mustbe (tk_rcb) ;
    pop_error_context() ;
    return create_subr_body (decs, body) ;
}

// var_decs ::= var_dec*
// returns ast_var_decs: create_var_decs(vector<ast> decs)
// create_var_decs must be passed a vector of ast_var_dec
//

ast parse_var_decs()
{
    push_error_context("parse_var_decs()") ;
    
    vector<ast> decs ;
    localspace = 0;

    while ( have (tk_var) ) // if there exist var's
    {
        vector<ast> temp = parse_var_dec () ;
        decs.insert( decs.end(), temp.begin(), temp.end() );
    }
    pop_error_context() ;
    return create_var_decs (decs) ;
}

// var_dec ::= 'var' type identifier (',' identifier)* ';'
// returns vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
vector<ast> parse_var_dec()
{
    vector<ast> decs ;
    push_error_context("parse_var_dec()") ;

    mustbe (tk_var) ;
    
    Token typeToken = parse_type () ;
    string type = token_spelling (typeToken) ;

    
    string name = token_spelling () ;
    mustbe (tk_identifier) ;

    ast var_dec = create_var_dec(name, "local", localspace, type) ;
    decs.push_back (var_dec) ;
    st_variable tableVar (name, type, "local", localspace) ;
    st_variable temp = lookup_variables (tempLclVars, name) ;
    if ( temp.offset == -1 )
    {
        temp = lookup_variables (tempArgVars, name) ;
        if ( temp.offset == -1 )
        {
            insert_variables (tempArgVars, name, tableVar) ;
        }
        else
        {
            fatal (name + " already declared in this scope");
        }
    }
    else
    {
        fatal(name+ " was already declared in this scope");
    }

    ++localspace;

    while ( have (tk_comma) )
    {
        mustbe (tk_comma) ;

        name = token_spelling () ;
        mustbe (tk_identifier) ;
        
        var_dec = create_var_dec(name, "local", localspace, type) ;
        decs.push_back (var_dec) ;
        st_variable tableVar (name, type, "local", localspace) ;
        
        st_variable temp = lookup_variables (tempLclVars, name) ;
        if ( temp.offset == -1 )
        {
            temp = lookup_variables (tempArgVars, name) ;
            if ( temp.offset == -1 )
            {
                insert_variables (tempArgVars, name, tableVar) ;
            }
            else
            {
                fatal (name + " already declared in this scope");
            }
        }
        else
        {
            fatal(name+ " was already declared in this scope");
        }
        ++ localspace ;
    }

    mustbe (tk_semi) ;

    pop_error_context() ;
    return decs ;
}

// statements ::= statement*
// create_statements(vector<ast> statements)
// create_statements must be passed a vector of ast_statement
//
ast parse_statements()
{
    push_error_context("parse_statements()") ;

    vector<ast> statements ;

    while (have (tk_statement) )
    {
        ast ast_statement = parse_statement () ;
        statements.push_back (ast_statement) ;
    }

    pop_error_context() ;
    return create_statements (statements) ;
}

// statement ::= let | if | while | do | return
// create_statement(ast statement)
// create_statement initialiser must be one of: ast_let, ast_let_array, ast_if, ast_if_else, ast_while, ast_do, ast_return or ast_return_expr
//
ast parse_statement()
{
    push_error_context("parse_statement()") ;
    ast ast_statement ;
    switch( token_kind() )
    {
        case tk_while:
        ast_statement = parse_while () ;
        pop_error_context() ;
        return create_statement (ast_statement) ;

        case tk_if:     
        ast_statement = parse_if () ;
        pop_error_context() ;
        return create_statement (ast_statement) ;

        case tk_let:
        ast_statement = parse_let () ;
        pop_error_context() ;
        return create_statement (ast_statement) ;

        case tk_do:
        ast_statement = parse_do () ;
        pop_error_context() ;
        return create_statement (ast_statement) ;

        case tk_return:
        ast_statement = parse_return();
        pop_error_context() ;
        return create_statement (ast_statement) ;
    }

    fatal ("parse_statement() found an unexpected statement kind " + token_spelling ()) ;
    pop_error_context() ;
    return -1 ;
}

// let ::= 'let' identifier index? '=' expr ';'
// return one of: ast_let or ast_let_array
//
// create_let(ast var,ast expr)
// . var: ast_var - the variable being assigned to
// . expr: ast_expr - the variable's new value
//
// create_let_array(ast var,ast index,ast expr)
// . var: ast_var - the variable for the array being assigned to
// . index: ast_expr - the array index
// . expr: ast_expr - the array element's new value
//
ast parse_let()
{
    push_error_context("parse_let()") ;
    mustbe (tk_let) ;
    bool array = false;
    string word = token_spelling ();
    mustbe (tk_identifier) ;

    st_variable tempVar = lookup_variables (stcVars, word);
    if ( tempVar.offset == -1 )
    {
        tempVar = lookup_variables (tempLclVars, word) ;
        if ( tempVar.offset == -1 )
        {
            tempVar = lookup_variables (tempArgVars, word);
            {
                if ( tempVar.offset == -1 )
                {
                    // as functions can't access field
                    if (parser_is_function) fatal ("not declared mate") ;

                    tempVar = lookup_variables (fldVars, word);
                    if ( tempVar.offset == -1 )
                    {
                        fatal ("not declared mate") ;
                    }
                }
            }
        }
    }
    ast var = create_var (tempVar.name, tempVar.segment, tempVar.offset, tempVar.type);
    ast index = ast_oops;

    if ( have (tk_lsb) )
    {
        array = true;
        index = parse_index () ;
    }

    mustbe (tk_eq) ;
    ast expr = parse_expr () ;
    mustbe (tk_semi) ;
    pop_error_context() ;

    if (array)
    {
        return create_let_array(var, index, expr) ;
    }
    return create_let(var, expr) ;
}

// if ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// return one of: ast_if or ast_if_else
//
// create_if(ast condition,ast if_true)
// . condition: ast_expr - the if condition
// . if_true: ast_statements - the if true branch
//
// create_if_else(ast condition,ast if_true,ast if_false)
// . condition: ast_expr - the if condition
// . if_true: ast_statements - the if true branch
// . if_false: ast_statements - the if false branch
//

// if ( condition ) statement ;
ast parse_if()
{
    push_error_context("parseIf()") ;

    mustbe (tk_if) ;
    mustbe (tk_lrb) ;
    ast condition = parse_expr () ;
    mustbe (tk_rrb) ;
    mustbe (tk_lcb) ;
    ast if_true = parse_statements () ;
    mustbe (tk_rcb) ;

    if (have (tk_else) )
    {
        mustbe (tk_else) ;
        mustbe (tk_lcb) ;
        ast if_false = parse_statements () ;
        mustbe (tk_rcb) ;
        pop_error_context() ;
        return create_if_else(condition, if_true, if_false) ;
    }
    
    pop_error_context() ;
    return create_if (condition, if_true) ;
}

// while ::= 'while' '(' expr ')' '{' statements '}'
// returns ast_while: create_while(ast condition,ast body)
// . condition: ast_expr - the loop condition
// . body: ast_statements - the loop body
//
ast parse_while()
{
    push_error_context("parse_while()") ;
    mustbe (tk_while) ;
    mustbe (tk_lrb) ;
    ast expr = parse_expr () ;
    mustbe (tk_rrb) ;
    mustbe (tk_lcb) ;
    ast statements = parse_statements () ;
    mustbe (tk_rcb) ;

    pop_error_context() ;
    return create_while (expr, statements) ;
}

// do ::= 'do' identifier (call | id_call) ';'
// returns ast_do: create_do(ast call)
// create_do must be passed one of: ast_call_as_function or ast_call_as_method
//
// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments
//
ast parse_do()
{
    push_error_context("parse_do()") ;
    mustbe (tk_do) ;

    string word = token_spelling () ;
    mustbe (tk_identifier) ;

    st_variable tempVar = lookup_variables (stcVars, word);
    if ( tempVar.offset == -1 )
    {
        tempVar = lookup_variables (tempLclVars, word) ;
        if ( tempVar.offset == -1 )
        {
            tempVar = lookup_variables (tempArgVars, word);
            {
                if ( tempVar.offset == -1 )
                {
                    // as functions can't access field
                    if (!parser_is_function) 
                        tempVar = lookup_variables (fldVars, word);
                }
            }
        }
    }

    // parse call id
    if ( have (tk_stop) ) 
    {
        ast call = parse_id_call () ;
        string word2 = get_subr_call_subr_name (call);
        ast subr_call = get_subr_call_expr_list (call);
        
        if (lookup_ints (methodNames, word2) != -1)
        {
            word = tempVar.type;
            if (word == "")
            {
                create_call_as_method(className, create_this (), call) ;
            }

            ast parent = create_var(tempVar.name,tempVar.segment,tempVar.offset,tempVar.type) ;
            pop_error_context () ;
            mustbe (tk_semi) ;
            return create_do( create_call_as_method(word, parent, call) ) ;
        }
        if (lookup_ints (functionNames, word2) != -1 || lookup_ints (constructors, word2) != -1 )
        {
            pop_error_context () ;
            mustbe (tk_semi) ;
            return create_do( create_call_as_function (word, call) ) ;
        }
        // treat word as function anyway?
        mustbe (tk_semi) ;
        return create_do( create_call_as_function (word, call) ) ;
    }

    // parse call
    if ( have (tk_lrb) ) 
    {
        ast call = parse_call () ;
        ast subr_call = create_subr_call (word, call) ;
        if (lookup_ints (functionNames, word) != -1 || lookup_ints (constructors, word) != -1 )
        {

            pop_error_context () ;
            return create_do(create_call_as_function (word, subr_call) ) ;
        }
        if (lookup_ints (methodNames, word) != -1)
        {

            pop_error_context () ;
            return create_do(create_call_as_method (className, create_this (), subr_call) ) ;
        }
        
        mustbe (tk_semi) ;
        return create_do( create_call_as_function (word, call) ) ;
    }

    fatal ("no function") ;
    return ast_oops ;
    
}

// return ::= 'return' expr? ';'
// returns one of: ast_return or ast_return_expr
//
// ast_return: create_return()
//
// ast_return_expr: create_return_expr(ast expr)
// create_return_expr must be passed an ast_expr
//
ast parse_return()
{
    mustbe (tk_return) ;
    push_error_context("parse_return()") ;

    if (have (tk_semi) )
    {
        mustbe (tk_semi);

        // if there should be a return
        if (! parser_is_void)
        {
            fatal ("no return") ;
        }
        pop_error_context() ;
        return create_return() ;
    }
    // if there shouldn't be a return
    if (parser_is_void)
    {
        fatal ("attempting to return non-void") ;
    }
    if (parser_is_constructor)
    {
        if( !have (tk_this) )
        {
            fatal ("attempting to return non-this for constructor") ;
        }
    }
    ast expr = parse_expr() ;
    mustbe (tk_semi);

    pop_error_context() ;
    return create_return_expr (expr) ;
}

// expr ::= term (infix_op term)*
// returns ast_expr: create_expr(vector<ast> expr)
// the vector passed to create_expr:
// . must be an odd length of at least 1, ie 1, 3, 5, ...
// . must be  a vector alternating between ast_term and ast_infix_op nodes, ending with an ast_term
//
ast parse_expr()
{
    push_error_context("parse_expr()") ;
    vector<ast> expr ;

    ast ast_term = parse_term ();
    expr.push_back (ast_term) ;

    while ( have (tk_infix_op) )
    {
        ast ast_infix_op = parse_infix_op ();

        ast_term = parse_term () ;
        
        expr.push_back (ast_infix_op) ;
        expr.push_back (ast_term) ;
    }

    if (expr.size () % 2 == 0)
    {
        fatal ("must be odd") ;
        return ast_oops ; // must be odd
    }   

    pop_error_context() ;
    return create_expr (expr) ;
}

// ast_int: create_int(int _constant)
// create_int must be passed an integer value in the range 0 to 32767
ast parse_int ()
{
    push_error_context("parse_int()") ;

    int res = -1 ;
    if (have (tk_integerConstant) )
    {
        res = stoi ( token_spelling () ) ;
    }
    mustbe(tk_integerConstant);
    ast result = create_int (res) ;

    pop_error_context() ;
    return result ;
}

// ast_string: create_string(string _constant)
// create_string must be passed any legal Jack string literal
ast parse_string ()
{
    push_error_context("parse_string()") ;
    
    string res = token_spelling () ;
    mustbe (tk_stringConstant) ;
    ast result = create_string (res) ;

    pop_error_context() ;
    return result;
}

// ast_bool: create_bool(bool t_or_f)
// create_bool must be passed true or false
ast parse_bool ()
{
    push_error_context ("parse_bool()") ;
    bool result ;

    if ( have (tk_true) )
    {
        mustbe (tk_true) ;
        result = true;
    }
    else if ( have (tk_false) )
    {
        mustbe (tk_false) ;
        result = false;
    }
    
    pop_error_context() ;
    return create_bool (result);
}

// ast_null: create_null()
ast parse_null ()
{
    push_error_context ("parse_null()") ;
    pop_error_context () ;
    mustbe (tk_null) ;
    return create_null () ;
}

// ast_this: create_this()
ast parse_this ()
{
    push_error_context("parse_this()") ;
    pop_error_context() ;
    mustbe (tk_this) ;
    return create_this () ;
}

// ast_unary_op: create_unary_op(string op,ast term)
// create_unary_op must be passed:
// . op: the unary op
// . term: ast_term
ast parse_unary_op ()
{
    push_error_context("parse_unary_op()") ;
    string op = token_spelling () ;
    mustbe (tk_unary_op) ;
    ast term = parse_term ();
    pop_error_context() ;
    return create_unary_op (op, term) ;
}

// term ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// returns ast_term: create_term(ast term)
// create_term must be passed one of: ast_int, ast_string, ast_bool, ast_null, ast_this, ast_expr,
//                                    ast_unary_op, ast_var, ast_array_index, ast_call_as_function, ast_call_as_method
//
ast parse_term ()
{
    push_error_context("parse_term()") ;
    // push_error_context(token_kind_to_string ( token_kind( current_token () )  ) ) ;
    
    ast term;
    switch ( token_kind() )
    {
        case tk_identifier : 
        term = parse_var_term () ;
        return create_term (term) ;

        case tk_integerConstant : 
        term = parse_int () ;
        return create_term (term) ;

        case tk_stringConstant : 
        term = parse_string () ;
        return create_term (term) ;
        
        case tk_true : 
        case tk_false :
        term = parse_bool () ;
        return create_term (term) ;
        
        case tk_null : 
        term = parse_null () ;
        return create_term (term) ;
        
        case tk_this : 
        term = parse_this() ;
        return create_term (term) ;
        
        case tk_lrb : 
        mustbe (tk_lrb);
        term = parse_expr() ;
        mustbe (tk_rrb);
        return create_term (term) ;
        
        case tk_sub :
        case tk_not : 
        term = parse_unary_op ();
        return create_term (term) ;
    }

    fatal("parse_term() has recieved unexpected input " + token_spelling ()) ;
    pop_error_context() ;
    return ast_oops ;
}

// var_term ::= identifier (index | id_call | call)?
// returns one of: ast_var, ast_array_index, ast_call_as_function or ast_call_as_method
//
// ast_var: create_var(string name,string segment,int offset,string type)
// create_ast_var must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
// ast_array_index: create_array_index(ast var,ast index)
// create_ast_array_index must be passed:
// . var: ast_var - the array variable
// . index: ast_expr - the index into the array
//
// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments
//

ast parse_var_term()
{
    push_error_context("parse_var_term()") ;

    string word = token_spelling () ;
    mustbe (tk_identifier) ;

    st_variable tempVar = lookup_variables (stcVars, word);
    if ( tempVar.offset == -1 )
    {
        tempVar = lookup_variables (tempLclVars, word) ;
        if ( tempVar.offset == -1 )
        {
            tempVar = lookup_variables (tempArgVars, word);
            {
                if ( tempVar.offset == -1 )
                {
                    // as functions can't access field
                    if (!parser_is_function) 
                        tempVar = lookup_variables (fldVars, word);
                }
            }
        }
    }
    // parse call id
    if ( have (tk_stop) ) 
    {
        ast call = parse_id_call () ;
        string word2 = get_subr_call_subr_name (call);
        ast subr_call = get_subr_call_expr_list (call);
        
        if (lookup_ints (methodNames, word2) != -1)
        {
            word = tempVar.type;
            if (word == "")
            {
                create_call_as_method(className, create_this (), call) ;
            }

            ast parent = create_var(tempVar.name,tempVar.segment,tempVar.offset,tempVar.type) ;
            pop_error_context () ;
            return create_call_as_method(word, parent, call) ;
        }
        if (lookup_ints (functionNames, word2) != -1 || lookup_ints (constructors, word2) != -1 )
        {
            pop_error_context () ;
            return create_call_as_function (word, call) ;
        }
        // treat word as function anyway?
        return create_call_as_function (word, call) ;
        fatal ("function/ method/ constructor " + word2 + " not declared in this scope") ;
        return ast_oops;
    }

    // parse call
    if ( have (tk_lrb) ) 
    {
        if (lookup_ints (functionNames, word) != -1 || lookup_ints (constructors, word) != -1 )
        {
            ast call = parse_call () ;
            ast subr_call = create_subr_call (word, call) ;
            pop_error_context () ;
            return create_call_as_function (word, subr_call) ;
        }
        if (lookup_ints (methodNames, word) != -1)
        {
            ast call = parse_call () ;
            ast subr_call = create_subr_call (word, call) ;
            pop_error_context () ;
            return create_call_as_method (className, create_this (), subr_call) ;
        }
        fatal ("function/ method " + word + " not declared in this scope") ;
        return ast_oops;
    }

    // parse index
    if ( have (tk_lsb) ) 
    {
        ast var = create_var(tempVar.name, tempVar.segment, tempVar.offset, tempVar.type) ;
        ast index = parse_index () ;
        pop_error_context () ;
        return create_array_index (var, index) ;
    }

    if (tempVar.offset == -1) 
    {
        fatal (word + " has not been declared in this scope") ;
        return ast_oops ;
    }
    pop_error_context () ;
    return create_var(tempVar.name, tempVar.segment, tempVar.offset, tempVar.type) ;
}

// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments

// index ::= '[' expr ']'
// returns ast_expr
ast parse_index()
{
    push_error_context("parse_index()") ;

    mustbe (tk_lsb) ;
    ast expr = parse_expr () ;
    mustbe (tk_rsb) ;

    pop_error_context() ;
    return expr ;
}

// id_call ::= '.' identifier call
// returns ast_subr_call: create_subr_call(string subr_name,ast expr_list)
// create_subr_call must be passed:
// . subr_name: the constructor, function or method's name within its class
// . expr_list: ast_expr_list - the explicit arguments to the call
//
ast parse_id_call()
{
    push_error_context("parse_id_call()") ;

    mustbe (tk_stop);
    string subr_name = token_spelling ();

    mustbe (tk_identifier) ;
    ast expr_list = parse_call ();

    pop_error_context() ;
    return create_subr_call(subr_name, expr_list) ;
}

// call ::= '(' expr_list ')'
// returns ast_expr_list
//
ast parse_call()
{
    push_error_context("parse_call()") ;

    mustbe (tk_lrb) ;
    ast list = parse_expr_list () ;
    mustbe (tk_rrb) ;
    pop_error_context() ;

    return list ;
}

// expr_list ::= (expr (',' expr)*)?
// returns ast_expr_list: create_expr_list(vector<ast> exprs)
// create_expr_list must be passed: a vector of ast_expr
//
ast parse_expr_list()
{
    push_error_context("parse_expr_list()") ;
    vector<ast> exprs ;
    if (have (tk_term) )
    {
        ast expr = parse_expr ();
        exprs.push_back (expr) ;

        while ( have (tk_comma) )
        {            
            mustbe (tk_comma) ;
            expr = parse_expr ();
            exprs.push_back (expr) ;
        }
    }
    pop_error_context() ;
    return create_expr_list (exprs) ;
}

// infix_op ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// returns ast_op: create_infix_op(string infix_op)
// create_infix_op must be passed:
// infix_op: the infix op
//
ast parse_infix_op()
{
    push_error_context ("parse_infix_op()") ;
    ast temp = create_infix_op ( token_spelling () );
    mustbe (tk_infix_op) ;
    pop_error_context () ;
    return temp ;
}

ast jack_parser()
{
    // read the first token to get the tokeniser initialised
    next_token() ;

    // construct tree and return as result
    return parse_class() ;
}

// main program
int main(int argc,char **argv)
{
    // parse a Jack class and print the abstract syntax tree as XML
    ast_print_as_xml(jack_parser(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

