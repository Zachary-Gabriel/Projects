// test token groups and context strings
#include "tokeniser.h"
#include <iostream>
#include <vector>

using namespace std ;
using namespace Assignment_Tokeniser ;

// token_is_in test
void token_is_in_test(Token token,TokenKind kind)
{
    cout << "    token_is_in(token," << token_kind_to_string(kind) << ") returns " ;
    cout << (token_is_in(token,kind) ? "true" : "false") ;
    cout << endl;
}

// this main program tokenises standard input and
// prints details of every 5th token in reverse order
int main(int argc,char **argv)
{
    Token token ;
    vector<Token> tokens ;
    int count = 0 ;

    // remember each token as it is read
    token = next_token() ;
    while ( token_kind(token) != tk_eoi )
    {
        tokens.push_back(token) ;
        token = next_token() ;
        count++ ;
    }
    cout << "Read " << count << " tokens" << endl << endl ;

    // display every 5th token context in reverse order
    cout << "Token Contexts:" << endl << endl ;
    while ( --count >= 0 )
    {
        if ( count % 5 != 0 ) continue ;

        // the token to be interrogated
        token = tokens[count] ;

        // its string representation
        cout << "token: " << count << endl ;
        cout << "    " << token_to_string(token) << endl ;

        // check it against some group memberships
        cout << "token group memberships:" << endl ;
        token_is_in_test(token,tk_identifier) ;
        token_is_in_test(token,tk_integer) ;
        token_is_in_test(token,tk_float) ;
        token_is_in_test(token,tk_keyword) ;
        token_is_in_test(token,tk_symbol) ;

        // its context string
        cout << "token_context:" << endl ;
        cout << token_context(tokens[count]) << endl ;
        cout << "----------------" << endl ;
    }

    return 0 ;
}
