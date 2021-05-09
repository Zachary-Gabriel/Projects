#ifndef ASSIGNMENT_TOKENISER_H
#define ASSIGNMENT_TOKENISER_H

#include <string>

// Interface to the assignment tokeniser
namespace Assignment_Tokeniser
{
    // a shorthand for string
    typedef std::string string ;

    // the type of a token returned by the tokeniser
    typedef int Token ;

    // ***** BNF *****
    //
    // *** SYNTAX
    // * literals inside ' '
    // * grouping ( )
    // * ASCII ranges -
    // * alternatives |
    // * 0 or 1 ?
    // * 0 or more *
    // * 1 or more +
    //
    // *** TOKEN RULES
    // * identifier ::= (letter|'_') (letter|digit|'_'|'$'|'.')*
    //   - letter ::= 'a'-'z'|'A'-'Z'
    //
    // * integer ::= zero | digits
    //   - zero ::= '0'
    //   - digits ::= digit19 digit*
    //   - digit19  ::= '1'-'9'
    //   - digit  ::= '0'-'9'
    //
    // * float ::= (zero | digits) ((fraction exponent?) | exponent)
    //   - fraction ::= '.' digit*
    //   - exponent ::= ('e'|'E') ('+'|'-')? (zero | digits)
    //
    // * string ::= '"' instring* '"'
    //   - instring ::= ' '|'!'|'#'-'~'
    //
    // * keyword ::= 'do'|'for'|'pointer'|'real'|'this' 
    //   - keywords each have their own TokenKind
    //
    // * symbol ::= '@'|';'|':'|'!'|','|'.'|'=='|'<=>'|'}'|'{'|')'|'('|']'|'['|'/'
    //   - symbols each have their own TokenKind
    //
    // * eol_comment ::= '//' eol_comment_char* '\n'
    //   - eol_comment_char ::= ' '-'~'
    //
    // * adhoc_comment ::= '/*' adhoc_comment_char* '*/'
    //   - adhoc_comment_char ::= eol_comment_char|'\n'
    //
    // ** INPUT PREPROCESSING
    // * nextch() replaces tab characters with one or more spaces so that the current line length becomes a multiple of four characters
    // * nextch() never sets ch to be a tab character
    // * nextch() discards carriage return characters that are immediately followed by a newline character
    // * nextch() replaces carriage return characters that are not immediately followed by a newline character with a newline character
    // * nextch() never sets ch to be a carriage return character
    // * only characters assigned to ch can appear in the results of calls to token_context()
    //
    // ** OTHER RULES
    // * the '"' and '"' enclosing a string do not appear in the token's spelling
    // * the '//' and '\n' enclosing an eol_comment do not appear in the token's spelling
    // * the '/*' and '*/' enclosing an adhoc_comment do not appear in the token's spelling
    // * the token's spelling for a float must be modified as follows:
    //   * if there are no digits after the '.', the '.' is replaced with '.0'
    //   * if there is no exponent, 'e+0' is added to the end of the spelling
    //   * if there is an exponent, 'E' is replaced by 'e'
    //   * if there is an exponent but no sign, 'e' is replaced by 'e+'
    //   * any modifications are not displayed in the results of calls to token_context()
    // * all the keywords satisfy the grammar rule for an identifier
    //   * keywords must be identified by
    //     1. recognising an identifier
    //     2. calling string_to_token_kind() to get a TokenKind for the identifier
    //     3. calling a function like token_is_in() to see if the TokenKind returned belongs to a keyword
    //   * when new_token() is asked to create a keyword token it will check that the last call to string_to_token_kind()
    //     was passed the spelling of the token being created
    //
    // ** ERROR HANDLING
    // * when looking for the start of a token,
    //   * space and newline characters are ignored
    //   * any other character that cannot start a token, including the EOF marker, causes the tk_eoi token to be returned
    // * if the next character cannot extend the current token,
    //   * if the characters read so far can form a token, that token is returned
    //   * if the characters read so far cannot form a token, the tk_eoi token is returned
    // * if the tk_eoi token is returned,
    //   * ch must be set to the EOF marker to simulate end of input
    //   * all future calls to nextch() will not read any more input
    //   * all future calls to next_token() will return the tk_eoi token
    //


    // The kinds of token that are recognised
    enum TokenKind
    {
        tk_eol_comment,     // '//' eol_comment_char* '\n', '//' and '\n' are not part of the spelling
        tk_adhoc_comment,   // '/*' adhoc_comment_char* '*/', '/*' and '*/' are not part of the spelling

        tk_identifier,      // identifier

        tk_integer,         // integer
        tk_float,           // float
        tk_string,          // '"' instring* '"', '"' and '"' are not part of the spelling

        tk_do,              // 'do'
        tk_for,             // 'for'
        tk_pointer,         // 'pointer'
        tk_real,            // 'real'
        tk_this,            // 'this'


        tk_at,              // '@'
        tk_semi,            // ';'
        tk_colon,           // ':'
        tk_not,             // '!'
        tk_comma,           // ','
        tk_stop,            // '.'
        tk_eq,              // '=='
        tk_spaceship,       // '<=>'
        tk_lcb,             // '{'
        tk_rcb,             // '}'
        tk_lrb,             // '('
        tk_rrb,             // ')'
        tk_lsb,             // '['
        tk_rsb,             // ']'
        tk_div,             // '/'

        tk_eoi,             // end of input reached

                            // the remaining tokens are not produced by the tokeniser
                            // these are provided to describe groups of tokens

        tk_number,          // one of tk_integer or tk_float
        tk_keyword,         // one of tk_do, tk_for, tk_pointer, tk_real or tk_this
        tk_symbol,          // one of tk_at, tk_semi, tk_colon, tk_not, tk_comma, tk_stop, tk_eq,
                            //        tk_spaceship, tk_lcb, tk_rcb, tk_lrb, tk_rrb, tk_lsb, tk_rsb or tk_div

        tk_oops             // for error handling
    } ;

    // ***** the following are implemented in the pre-compiled lib/.../lib.a object file *****

    // return a string representation of the given token kind
    extern string token_kind_to_string(TokenKind kind) ;

    // returns the TokenKind that would cause token_kind_to_string to return s, or tk_oops
    extern TokenKind string_to_token_kind(std::string s) ;

    // create a new Token object and return its ID
    extern Token new_token(TokenKind kind,string spelling,int start_line,int start_column) ;

    // return the TokenKind for the given token
    extern TokenKind token_kind(Token token) ;

    // return the characters that make up the given token
    extern string token_spelling(Token token) ;

    // return the line number where the given token was found, the first line is line number 1
    extern int token_line(Token token) ;

    // return the column position where the given token starts on its line, the first character is in column number 1
    extern int token_column(Token token) ;

    // return a string representation of the given token
    extern string token_to_string(Token token) ;

    // read the next character from the input, this may or may not be standard input during testing
    extern char read_char() ;

    // unread the last character returned by read_char(), it will be returned by the next call of read_char()
    extern void unread_char() ;

    // ***** the assignment involves implementing the following functions by completing tokeniser.cpp *****

    // is the token of the given kind or does it belong to the given grouping?
    // for example, token_is_in(new_token(tk_lsb,"["),tk_symbol) would return true
    extern bool token_is_in(Token token,TokenKind kind_or_grouping) ;

    // read the next token from standard input
    extern Token next_token() ;

    // generate a context string for the given token
    // it shows the line before the token,
    // the line containing the token up to the end of the token, and
    // a line with a ^ marking the token's position
    // the lines are prefixed by line numbers, right justified in four spaces, followed by ': '
    // if a token extends over more than one line, only the part of the token on the first line is included
    extern string token_context(Token token) ;
}
#endif //ASSIGNMENT_TOKENISER_H
