// tokeniser implementation for the workshop example language

#include "tokeniser.h"
#include <ctype.h>

// to shorten the code
using namespace std ;

// we are extending the Workshop_Tokeniser namespace

namespace Workshop_Tokeniser
{
    // parse a single character symbol
    // we know spelling is the one character symbol
    // the caller already knows the kind and spelling so they are passed as parameters
    // if they were not we would have to repeat the switch statement to find out
    static Token parse_single_char_symbol(TokenKind kind,string spelling)
    {
        // always read the next character - we have read past the end of the token
        nextch() ;

        // return a new Token object
        return new_token(kind,spelling) ;
    }

    // parse a number - always read one extra character
    // we know spelling is the first digit of the number
    static Token parse_integer(TokenKind kind,string spelling)
    {
        // always read the next character
        nextch() ;

        // append digits to spelling until we read past the end of the integer
        while ( isdigit(ch) )
        {
            spelling += ch ;
            nextch() ;
        }

        // return a new Token object
        return new_token(kind,spelling) ;
    }

    // parse end of input - report we have reached end of input
    // simulate end of input to prevent further attemps to read input
    static Token parse_eoi(TokenKind kind,string spelling)
    {
        // if called after an error we may not be at the end of input so simulate it
        ch = EOF ;

        // return a new Token object
        return new_token(kind,spelling) ;
    }

    // return the next Token object by reading more of the input
    // this function simply searches for the start of a token
    // then calls the appropriate parse function to complete the Token object
    Token next_token()
    {
        string spelling = "" ;

        while ( ch != EOF )	            // loop until EOF or the start of a token is found
        {
            spelling = ch ;             // initialise spelling with first character

            switch(ch)			        // ch is always the next char to read
            {
            case ' ':			        // ignore whitespace (space, tab, carriage return, line feed)
            case '\t':
            case '\r':
            case '\n':
                            nextch() ;  // read next character and go around again
                            break ;

                                        // when we find the first character of a token
                                        // call a function to read all of its characters
                                        // and construct an appropriate Token object

                                        // single character symbols
            case '{':       return parse_single_char_symbol(tk_lcb,spelling) ;
            case '}':       return parse_single_char_symbol(tk_rcb,spelling) ;

                                        // add new case labels to find the start of other Tokens
                                        // you may need to write additional parse_ functions
                                        // ...

                                        // you can use ranges such as '0' to '9'
            case '0'...'9': return parse_integer(tk_integer,spelling) ;

            default:			        // anything unexpected - pretend we reached the end of the input
                            return parse_eoi(tk_eoi,"") ;
            }
        }
        return parse_eoi(tk_eoi,"") ;            // assume we are at the end of the input
    }
}

