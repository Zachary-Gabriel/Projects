#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std;
using namespace CS_IO_Buffers;
using namespace Jack_Compiler;

// forward declarations of one function per node in the abstract syntax tree
void walk_class(ast t);
void walk_class_var_decs(ast t);
void walk_var_dec(ast t);
void walk_subr_decs(ast t);
void walk_subr(ast t);
void walk_constructor(ast t);
void walk_function(ast t);
void walk_method(ast t);
void walk_param_list(ast t);
void walk_subr_body(ast t);
void walk_var_decs(ast t);
void walk_statements(ast t);
void walk_statement(ast t);
void walk_let(ast t);
void walk_let_array(ast t);
void walk_if(ast t);
void walk_if_else(ast t);
void walk_while(ast t);
void walk_do(ast t);
void walk_return(ast t);
void walk_return_expr(ast t);
void walk_expr(ast t);
void walk_term(ast t);
void walk_int(ast t);
void walk_string(ast t);
void walk_bool(ast t);
void walk_null(ast t);
void walk_this(ast t);
void walk_unary_op(ast t);
void walk_var(ast t);
void walk_array_index(ast t);
void walk_call_as_function(ast t);
void walk_call_as_method(ast t);
void walk_subr_call(ast t);
void walk_expr_list(ast t);
void walk_infix_op(ast t);

// initialising global variables
static int indents = 0;
static bool is_if_or_while = false;
static string call_name = "";

// on a new line, add the new line character and appropriate indenting
void new_line ()
{
    write_to_output ("\n");

    for (int i = 0; i < indents; ++i)
    {
        write_to_output ( "    " );
    }
}

// wrapping the insides of the class inbetween curly brackets with a name declaration
void walk_class(ast t)
{
    string myclassname = get_class_class_name(t);
    ast var_decs = get_class_var_decs(t);
    ast subr_decs = get_class_subr_decs(t);

    write_to_output ("class " + myclassname + '\n' + "{");
    ++ indents;
    
    walk_class_var_decs(var_decs);
    walk_subr_decs(subr_decs);
    -- indents;

    write_to_output ("\n}");
    write_to_output ("\n");

}

// declaring the initial field and static variables
void walk_class_var_decs(ast t)
{
    int ndecs = size_of_class_var_decs(t);
    for ( int i = 0; i < ndecs; i++ )
    {
        // writting the variables:
        walk_var_dec(get_class_var_decs(t,i));
        write_to_output(";");

        // on the last line, add an extra line
        if (i == ndecs - 1)
        {
            write_to_output ("\n");
        }
    }
}

// writing out all the variables, which are different depending on which segment they're getting declared in
void walk_var_dec(ast t)
{
    string name = get_var_dec_name(t);
    string type = get_var_dec_type(t);
    string segment = get_var_dec_segment(t);
    int offset = get_var_dec_offset(t);

    if (segment == "local")
    {
        new_line ();
        write_to_output ("var " + type + ' ' + name);
    }
    else if (segment == "static")
    {
        new_line ();
        write_to_output ("static " + type + ' ' + name);
    }
    else if (segment == "this")
    {
        new_line ();
        write_to_output ("field " + type + ' ' + name);
    }
    else if (segment == "argument")
    {
        write_to_output (type + " " + name);
    }
    else // as a precaution
    {
        fatal_error (0,"unknown segment");
    }
}

// looping through all the subroutines
void walk_subr_decs (ast t)
{
    int size = size_of_subr_decs (t);
    for ( int i = 0; i < size; ++i )
    {
        // want to separate the subr decs by one line, except for the first
        if(i != 0)
        {
            write_to_output ("\n");
        }

        new_line ();
        walk_subr( get_subr_decs (t,i) );
    }
}

// parsing the subroutine
void walk_subr(ast t)
{
    ast subr = get_subr_subr(t);

    switch( ast_node_kind (subr) )
    {
        case ast_constructor:
        walk_constructor (subr);
        break;
        case ast_function:
        walk_function (subr);
        break;
        case ast_method:
        walk_method (subr);
        break;
        default:
        fatal_error (0,"Unexpected subroutine kind");
        break;
    }
}

// writing the constructor
void walk_constructor(ast t)
{
    string vtype = get_constructor_vtype(t);
    string name = get_constructor_name(t);
    ast param_list = get_constructor_param_list(t);
    ast subr_body = get_constructor_subr_body(t);

    // initialising the constructor
    write_to_output ( "constructor " + vtype + " " + name + "(" );

    // going through the params
    walk_param_list(param_list);
    
    // ending the params
    write_to_output (")");
    new_line ();

    // starting the meat and potatoes of the constructor with a lcb and indenting it
    write_to_output("{");
    walk_subr_body(subr_body);

    // closing the constructor with a rcb
    new_line ();
    write_to_output("}");
}

void walk_function(ast t)
{
    string vtype = get_function_vtype(t);
    string name = get_function_name(t);
    ast param_list = get_function_param_list(t);
    ast subr_body = get_function_subr_body(t);

    // initialising the function
    write_to_output ( "function " + vtype + " " + name + "(" );

    // going through the params
    walk_param_list(param_list);
    
    // ending the params
    write_to_output (")");
    new_line ();

    // starting the meat and potatoes of the function with a lcb and indenting it
    write_to_output("{");
    walk_subr_body(subr_body);

    // closing the function with a rcb
    new_line ();
    write_to_output("}");
}

void walk_method(ast t)
{
    string vtype = get_method_vtype(t);
    string name = get_method_name(t);
    ast param_list = get_method_param_list(t);
    ast subr_body = get_method_subr_body(t);

    // initialising the method
    write_to_output ( "method " + vtype + " " + name + "(" );

    // going through the params
    walk_param_list(param_list);
    
    // ending the params
    write_to_output (")");
    new_line ();

    // starting the meat and potatoes of the method with a lcb and indenting it
    write_to_output("{");
    walk_subr_body(subr_body);

    // closing the method with a rcb
    new_line ();
    write_to_output("}");
}

// go through the input variables
void walk_param_list(ast t)
{
    int ndecs = size_of_param_list(t);
    for ( int i = 0; i < ndecs; i++ )
    {
        walk_var_dec(get_param_list(t,i));

        // separate the inputs with commas
        if(i != ndecs -1)
        {
            write_to_output(",");
        }
    }
}

void walk_subr_body(ast t)
{
    ast decs = get_subr_body_decs(t);
    ast body = get_subr_body_body(t);

    walk_var_decs(decs);
    walk_statements(body);
}

void walk_var_decs(ast t)
{
    int ndecs = size_of_var_decs(t);

    ++ indents;
    for ( int i = 0; i < ndecs; ++i )
    {
        // go through inputs and stick a semicolon on the end
        walk_var_dec (get_var_decs(t,i));
        write_to_output (" ;");

        // add an extra line after all the declerations
        if(i == ndecs-1){
            write_to_output("\n");
        }
    }
    -- indents;
}

void walk_statements(ast t)
{
    int nstatements = size_of_statements(t);

    ++ indents;
    for ( int i = 0; i < nstatements; ++i )
    {
        // walking through each statement, starting each off with a new line
        new_line ();
        walk_statement(get_statements(t,i));

        // if and while statements have an extra line for code style, which the other statements don't
        ast future_statement = ast_node_kind(get_statement_statement(get_statements(t,i)));
        if(i != nstatements -1 && (future_statement == ast_if_else || future_statement == ast_while || future_statement == ast_if ) )
        {
            write_to_output ("\n");
            is_if_or_while = false;
        }
    }
    -- indents;
}

void walk_statement(ast t)
{
    ast statement = get_statement_statement (t);

    switch ( ast_node_kind (statement) )
    {
        case ast_let:
        walk_let (statement);
        break;
        case ast_let_array:
        walk_let_array (statement);
        break;
        case ast_if:
        walk_if (statement);
        break;
        case ast_if_else:
        walk_if_else (statement);
        break;
        case ast_while:
        walk_while (statement);
        break;
        case ast_do:
        walk_do (statement);
        break;
        case ast_return:
        walk_return (statement);
        break;
        case ast_return_expr:
        walk_return_expr (statement);
        break;
        case ast_statements:
        walk_statements (statement);
        break;
        default:
        fatal_error(0,"Unexpected statement kind");
        break;
    }
}

// write out let statement
void walk_let (ast t)
{
    ast var = get_let_var(t);
    ast expr = get_let_expr(t);

    write_to_output ("let ");
    walk_var (var);
    write_to_output (" = ");
    walk_expr (expr);
    write_to_output (" ;");
}

// write out the let array statement
void walk_let_array(ast t)
{
    ast var = get_let_array_var(t);
    ast index = get_let_array_index(t);
    ast expr = get_let_array_expr(t);

    // getting the array's name
    write_to_output ("let ");
    walk_var (var);

    // getting the array's index
    write_to_output ("[");
    walk_expr (index);
    write_to_output ("] = ");

    // declaring what the value will be
    walk_expr (expr);
    write_to_output (";");
}

// write out the if statement
void walk_if(ast t)
{
    ast condition = get_if_condition(t);
    ast if_true = get_if_if_true(t);

    // writing the if line out
    write_to_output ("if (");
    walk_expr (condition);
    write_to_output (")");
    new_line ();

    // wrapping the meat and potatoes of the if statement inside curly brackets
    write_to_output ("{");
    walk_statements (if_true);
    new_line ();
    write_to_output ("}");
}

// write out the if-else statements
void walk_if_else(ast t)
{
    ast condition = get_if_else_condition(t);
    ast if_true = get_if_else_if_true(t);
    ast if_false = get_if_else_if_false(t);

    // writing the if line out
    write_to_output ("if (");
    walk_expr (condition);
    write_to_output (")");
    new_line ();

    // wrapping the meat and potatoes of the if statement inside curly brackets
    write_to_output ("{");
    walk_statements (if_true);
    new_line ();
    write_to_output ("}");
    new_line ();

    // wrapping the meat and potatoes of the else statement inside curly brackets
    write_to_output ("else");
    new_line ();
    write_to_output ("{");
    walk_statements (if_false);
    new_line ();
    write_to_output ("}");
}

// writing out the while loop
void walk_while(ast t)
{
    ast condition = get_while_condition(t);
    ast body = get_while_body(t);

    // writing out the while condition
    write_to_output ("while (");
    walk_expr (condition);
    write_to_output (")");
    new_line ();

    // wrapping the while body in curly brackets
    write_to_output ("{");
    walk_statements (body);
    new_line ();
    write_to_output ("}");
}

void walk_do(ast t)
{
    ast call = get_do_call (t);

    switch (ast_node_kind(call))
    {
        case ast_call_as_function:
        walk_call_as_function (call);
        break;
        case ast_call_as_method:
        walk_call_as_method (call);
        break;
        default:
        fatal_error (0,"Unexpected call kind");
        break;
    }
}

// writing void return
void walk_return(ast t)
{
    // writing out the void's return
    write_to_output ("return ;");
}

// writing return with an expression
void walk_return_expr(ast t)
{
    ast expr = get_return_expr(t);

    // writing out the return + the expression
    write_to_output ("return ");
    walk_expr (expr);
    write_to_output (" ;");
}

// parsing the expressions
void walk_expr(ast t)
{
    int term_ops = size_of_expr (t);
    for ( int i = 0; i < term_ops; ++i )
    {
        ast term_op = get_expr (t,i);
        if ( i % 2 == 0 )
        {
            walk_term (term_op);
        }
        else
        {
            walk_infix_op(term_op);
        }
    }
}

void walk_term(ast t)
{
    ast term = get_term_term(t);

    switch(ast_node_kind(term))
    {
        case ast_int:
        walk_int(term);
        break;
        case ast_string:
        walk_string(term);
        break;
        case ast_bool:
        walk_bool(term);
        break;
        case ast_null:
        walk_null(term);
        break;
        case ast_this:
        walk_this(term);
        break;
        case ast_expr:
        walk_expr(term);
        break;
        case ast_unary_op:
        walk_unary_op(term);
        break;
        case ast_var:
        walk_var(term);
        break;
        case ast_array_index:
        walk_array_index(term);
        break;
        case ast_call_as_function:
        walk_call_as_function(term);
        break;
        case ast_call_as_method:
        walk_call_as_method(term);
        break;
        default:
        fatal_error(0,"Unexpected term kind");
        break;
    }
}

// writing constant int
void walk_int(ast t)
{
    int _constant = get_int_constant (t);
    // write the string of the int
    write_to_output( to_string (_constant) );
}

// writes constant string
void walk_string(ast t)
{
    string _constant = get_string_constant(t);
    // nesting the string inside quotation marks
    write_to_output('"' + _constant + '"');
}

// writes true or false
void walk_bool(ast t)
{
    bool _constant = get_bool_t_or_f (t);

    // writes true or false depending of the if its true or false
    if (_constant)
    {
        write_to_output ("true");
    }
    else
    {
        write_to_output ("false");
    }
}

// writes the word "null"
void walk_null(ast t)
{
    write_to_output ("null");
}

// writes the word "this"
void walk_this(ast t)
{
    write_to_output ("this");
}

// writes the unary operator
void walk_unary_op(ast t)
{
    string uop = get_unary_op_op (t);
    ast term = get_unary_op_term (t);

    write_to_output (uop);
    walk_term (term);
}

// writes the var's name
void walk_var(ast t)
{
    string name = get_var_name (t);
    string type = get_var_type (t);
    string segment = get_var_segment (t);
    int offset = get_var_offset (t);
    
    write_to_output (name);
}

// nesting the array index inside square brackets
void walk_array_index(ast t)
{
    ast var = get_array_index_var(t);
    ast index = get_array_index_index(t);

    walk_var (var);
    write_to_output ("[");
    walk_expr (index);
    write_to_output ("]");
}

// writing out the class' name (of the appropriate function)
void walk_call_as_function(ast t)
{
    string class_name = get_call_as_function_class_name (t);
    ast subr_call = get_call_as_function_subr_call (t);
    write_to_output (class_name + ".");
    walk_subr_call (subr_call);
}

// parsing method
void walk_call_as_method(ast t)
{
    string class_name = get_call_as_method_class_name(t);
    ast var = get_call_as_method_var(t);
    ast subr_call = get_call_as_method_subr_call(t);

    switch(ast_node_kind(var))
    {
        case ast_this:
        walk_this(var);
        break;
        // adding a dot between the variables name and its method call
        case ast_var:
        walk_var(var);
        write_to_output(".");
        break;
        default:
        fatal_error(0,"Expected var or this");
        break;
    }
    walk_subr_call(subr_call);
}

// writing out the function's / method's / constructor's call itself
void walk_subr_call(ast t)
{
    string subr_name = get_subr_call_subr_name(t);
    ast expr_list = get_subr_call_expr_list(t);

    // nesting call's name with brackets and its inputs
    call_name = subr_name;
    write_to_output (call_name + '(');
    walk_expr_list (expr_list);
    write_to_output (")");
}

// writing out the input expressions by separating them by commas
void walk_expr_list(ast t)
{
    int nexpressions = size_of_expr_list(t);
    for ( int i = 0; i < nexpressions; ++i )
    {
        if(i != 0)
        {
            write_to_output(",");
        }
        walk_expr(get_expr_list(t,i));   
    }
}

// writing out the operation's character
void walk_infix_op(ast t)
{
    string op = get_infix_op_op(t);
    write_to_output(' ' + op + ' ');
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and pretty print equivalent Jack code
    walk_class(ast_parse_xml());

    // flush the output and any errors
    print_output();
    print_errors();
}

