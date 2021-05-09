// a skeleton implementation of a tokeniser

#include "tokeniser.h"
#include <iostream>
#include <ctype.h>
#include <vector>

// to shorten the code
using namespace std ;

////////////////////////////////////////////////////////////////////////

namespace Assignment_Tokeniser
{

    // is the token of the given kind or does it belong to the given grouping?
    bool token_is_in(Token token,TokenKind kind_or_grouping)
    {
        TokenKind kind = token_kind(token) ;

        // check identity first
        if ( kind == kind_or_grouping ) return true ;

        // this is best written as nested switch statements
        switch(kind_or_grouping)
        {
            case tk_number: // a number can be both an integer or a float
            switch (kind)
            {
                case tk_integer:
                case tk_float: return true;
                default:
                return false ;
            }

            case tk_symbol: 
            switch (kind) // ie, if its an @ symbol, it is both a tk_symbol and a tk_at
            {
                case tk_at:
                case tk_semi:
                case tk_colon:
                case tk_not:
                case tk_comma:
                case tk_stop:
                case tk_lcb: 
                case tk_rcb: 
                case tk_lrb: 
                case tk_rrb:
                case tk_lsb:
                case tk_rsb:
                case tk_div: 
                case tk_spaceship:
                case tk_eq: return true;
                default:
                return false ;
            }

            case tk_keyword:
            switch (kind)
            {
                case tk_do:
                case tk_for: 
                case tk_pointer:
                case tk_real:
                case tk_this: return true;
                default:
                return false ;
            }

            default:
            return false ;
        }
    }

    // the current input character, initiliased to ' ' which we ignore
    // the eof marker can be confused with a legal character but not one accepted by our tokeniser
    static char ch = ' ' ;

    // the current line number and column, initialised to line 1 column 0
    static int line_num = 1 ;
    static int column_num = 0 ;

    // the line number and column for the first character in the current token
    static int start_line = 0 ;
    static int start_column = 0 ;

    // for token_context
    static vector<string> everything; // the vector containing all the lines of input
    static string contentLine = ""; // the string that contains line of input

    // generate a context string for the given token
    // it shows the line before the token,
    // the line containing the token up to the end of the token, and
    // a line with a ^ marking the token's position
    // the lines are prefixed by line numbers, right justified in four spaces, followed by ': '
    // if a token extends over more than one line, only the part of the token on the first line is included
    string token_context(Token token)
    {
        everything.push_back(contentLine); //add the last line to the vector
        int line = token_line (token) -1 ; // line of start point
        int col = token_column (token) ; // column of start point
        string lineSpelling = everything.at (line) ; // the line that where the token is found
        string tokenSpelling = token_spelling (token) ; // the spelling of the token itself
        string result = ""; // what will be added to

        //first line
        if (line >= 1) // if there should exist a first line
        {
            if (line < 10) // checking if we need two or three spaces
            {
                result += "   " + to_string(line) + ": " + everything.at(line-1) + '\n' ; // add the previous line
            }
            else
            {
                result += "  " + to_string(line) + ": " + everything.at(line-1) + '\n' ; // add the previous line
            }
        }

        // second line
        if (line < 9) // checking if we need two or three spaces
        {
            result += "   " + to_string(line+1) + ": " ; // marking out four spaces then adding the line number
        }
        else
        {
            result += "  " + to_string(line+1) + ": " ; // marking out four spaces then adding the line number
        }

        int filler = 0; // as some of the tokesn have had filler added to them. This exists to push past the filler.
        bool get_out = false; // terminates the for loop if bad index i.e. leaving the array
        for (int i = 0; i < lineSpelling.size(); i ++)
        {
            if(get_out) break; // error check so that we aren't leaving the bounds of the arrays (strings)
            if (i < col) // leading characters
            {
                result += lineSpelling[i] ; // add characters that lead up to the token
            } 
            else // we're now looking at the input that made our token
            {
                if (i+filler-col >= tokenSpelling.size()) // if our index leaves the bounds of the array, break the loop because we're likely done
                {
                    get_out = true; // to break the for loop
                    break;
                }

                if (lineSpelling[i] == tokenSpelling[i+filler-col] || (lineSpelling[i] == 'E' && tokenSpelling[i] == 'e')) // checking if the elements of spelling are the same as the elements of this input line. (and for the special case of E --> e)
                {
                    result += lineSpelling[i] ; 
                }
                else //if it doesn't follow spelling, then we likely need to increment the filler index.
                {
                    while(!(lineSpelling[i] == tokenSpelling[i+filler-col] || (lineSpelling[i] == 'E' && tokenSpelling[i] == 'e'))) // checking if the elements of spelling are the same as the elements of this input line. (and for the special case of E --> e)
                    {
                        filler++;
                        if (i+filler-col >= tokenSpelling.size()) // if our index leaves the bounds of the array, break the loop because we're likely done
                        {
                            get_out = true; // to break the for loop
                            break ; // breaks this while loop, so we aren't trapped
                        }
                    }
                    if(get_out) break; // error check so that we aren't leaving the bounds of the arrays (strings)
                    result += lineSpelling[i] ; // adds to result if allowed / exists
                }
            } 
        }

        //third line
        result +=  '\n';

        // if the line number itself is 2 digits, then this needs to spaces, otherwise one should suffice
        if (line > 9) result +="  ";
        else result += " ";

        // adding space for the 4 spaces at the front, and two that occupy ": "
        for (int i = 0; i < 4 + col; i++) // then also adding upto where col is 
        {
            result += " "; // marking out the spaces
        }
        result += '^'; // adding the caret
        
        result += '\n';// because the hidden rule was to have another line at the end
        return result ; // returning this mess
    }

    // read next character if not at the end of input
    // and update the line and column numbers
    static void nextch()
    {
        if ( ch == EOF )
        {
            return ;  
        }

        if ( ch == '\n' )           // if last ch was newline ...
        {
            line_num++ ;            // increment line number
            everything.push_back(contentLine); // add to the vector on new line

            column_num = 0 ;        // reset column number
            contentLine = ""; // start string again
        } else if (line_num == 1 && column_num == 0); //because the first character isn't apart of the input
        else contentLine += ch; // add to the collection

        ch = read_char() ;          // read the next character, probably from stdin but this could change during testing
        column_num++ ;              // increment the column number

        if (ch == '\t') // if tab
        {
            while (ch == '\t') // if multiple tabs follow
            {
                for (int i = 0; i < 4-(column_num % 4); i++) // for token_context. adds up to 4 spaces to the string that represents this line.
                {
                    contentLine += " ";
                }
                column_num += 4 - (column_num % 4) ; // add up to 4 spaces depending on how far along the row we are
                ch = read_char();
            }
            contentLine += " "; // adding another space for the next character to go.
            column_num++ ; // adding another space for the next character to go.
        }
        if (ch == '\r') // if carriage return character
        {
            char next = read_char(); // wanna see the next character
            if (next == '\n') // if its a new line, throw away \r
            {
                unread_char(); // want to unsee the next character and move along
                nextch(); // then call this function again
            }
            else // if it isnt a new line, make it a new line and throw away \r
            {
                unread_char(); // unsee the character
                ch = '\n'; // turn the\r into \n
                nextch(); //  then call this function again
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////

    // called when we find end of input or we an error
    Token parse_eoi()
    {
        // simulate end of input in case this is handling a bad token rather than a real end of input
        ch = EOF ;

        // return an eoi token
        return new_token(tk_eoi,"",start_line,start_column) ;
    }

    // if the character is a number return true
    bool isdigit (char val) 
    {
        if (48 <= int(val)&& int(val) <= 57 ) // checking if the character is between 0 -> 9
        {
            return true;
        }
        return false;
    }
    Token parse_exponent(string spelling)
    {
        spelling += 'e'; // add e to the number
        nextch();
        switch (ch) // checking the following character
        {
            case '+':
            case '-': // add its sign
            spelling += ch;
            nextch();
                if (ch == '0') // if it is zero
                {
                    spelling += '0'; // turning spelling e+ --> e+0
                    nextch();
                    return new_token (tk_float, spelling, start_line, start_column);
                }
                break;

            case '0':  // similar story to above if it starts with zero, it ends "e+0"
            spelling += '+';
            spelling += '0';
            nextch();
            return new_token (tk_float, spelling, start_line, start_column);

            case '1'...'9': // if it is an element of digits...
            spelling += '+'; // add its sign
            spelling += ch; // then the value
            nextch();
            break;

            default:
            return parse_eoi() ;;
        }

        while ( isdigit(ch) ) // if the next character is a number, keep it coming
        {
            spelling += ch; // pin it to the string
            nextch(); // grab the next number
        }
        return new_token (tk_float, spelling, start_line, start_column); // pin the number to a token
    }

    Token parse_decimal(string spelling) // if the number has a . in it
    {
        spelling += ch; // add .
        nextch();
        switch (ch) // then add the following digits/ exponents to the number
        {
            case '0'...'9': // if you're a digit, get slapped onto the end of the number
            spelling += ch;
            nextch();
            break;

            case 'e': // if youre an e/E, the the following digits will be apart of the exponent numbers
            case 'E':
            spelling += "0";
            return parse_exponent (spelling);

            default: // you're not a number or E/e, so we've finished the number.
            spelling += "0e+0"; 
            nextch();
            return new_token (tk_float, spelling, start_line, start_column);
        }
        while ( isdigit(ch) ) // if the next character is a number, keep it coming
        {
            spelling += ch; // pin it to the string
            nextch(); // grab the next number
        }
        if (ch == 'e'|| ch == 'E') // if the e comes later in the float, keep the current value, but parse it is an exponent again
        {
            return parse_exponent(spelling);
        }
        else
        {
            spelling += "e+0"; // you've got no e's or E's anywhere, so it gets added on at the end
            return new_token (tk_float, spelling, start_line, start_column); // pin the number to a token
        }
    }

    Token parse_float(TokenKind kind, string spelling) // if the integer has a 
    {
        switch (ch) // ch needed to be e/E or .
        {
            case 'e':
            case 'E': 
            return parse_exponent(spelling); // the e/E case:

            case '.': 
            return parse_decimal(spelling); // the . case:
        }  
    }

    Token parse_zero_integer (TokenKind kind, string spelling) // if the character is zero
    {
        spelling = ch; // pin zero to the string
        nextch(); // grab the next character for the sake of the program
        if (ch == '.' || ch == 'e' || ch == 'E')
        {
            return parse_float(kind, spelling);
        }
        return new_token (kind, spelling, start_line, start_column); // make the token with zero
    }

    // if the first character was a number between 1 -> 9
    Token parse_integer (TokenKind kind, string spelling)
    {
        while ( isdigit(ch) ) // if the next character is a number, keep it coming
        {
            spelling += ch; // pin it to the string
            nextch(); // grab the next number
        }
        if (ch == '.' || ch == 'e' || ch == 'E')
        {
            return parse_float(kind, spelling);
        }
        return new_token (kind, spelling, start_line, start_column); // pin the number to a token
    }

    Token parse_keyword(TokenKind kind, string spelling) // parsing a keyword found from the parse_identifier() function
    {
        Token newToken = new_token (kind, spelling, start_line, start_column); // making a temp token to see if if we have a keyword
        if (token_is_in(newToken, tk_keyword) ) //comparing tk_keyword with our new keyword
        {
            return newToken ; 
        }
        return new_token (tk_identifier, spelling, start_line, start_column);
    }

    //if the first character was a letter
    Token parse_identifier (TokenKind kind, string spelling)
    {
        while ( isdigit(ch) || 97 <= int(ch)&& int(ch) <= 122 ||  65 <= int(ch)&& int(ch) <= 90 || ch == '_' || ch == '.' || ch == '$')
        // if the following character is a number, a -> z, A -> Z, ., _ or $
        {
            spelling += ch; // pin the character to the string
            nextch(); // grab the next character
        }
        
        if ( string_to_token_kind(spelling) != tk_oops) // recognising an identfier
        {
            return parse_keyword(string_to_token_kind(spelling), spelling);
        }

        //end the loop if the next character is not part of indertifier grammar
        return new_token (tk_identifier, spelling, start_line, start_column);
    }

    Token parse_string(TokenKind kind, string spelling) // parsing string
    {
        nextch();// as ch was previously "
        while( ch!='"' && ch != EOF) // if its between quotes then it can enter (the grammar rules weren't clear on this one)
        {
            spelling += ch; // adding characters to string
            nextch(); // getting new character
        }
        nextch(); // so that the next character isn't "
        return new_token(kind, spelling, start_line, start_column);
    }

    Token parse_adhoc_com(TokenKind kind, string spelling) // parsing adhoc comment
    {
        nextch(); // as previous one was '*'
        char prev = '/'; // the previous value of ch
        while( !(prev == '*' && ch == '/') && ch != EOF)
        {
            spelling += ch; // add ch to the string
            prev = ch; // set the previous value to the current value
            nextch(); // find the next value
        }
        spelling = spelling.substr(0, spelling.size()-1); // to remove the * that was added before to find the value of prev
        nextch(); // to find the next char after /
        return new_token(kind, spelling, start_line, start_column); // setting the value of the string to an adhoc comment
    }

    Token parse_eol_com(TokenKind kind, string spelling) // parsing end of line comment
    {
        nextch(); // as previous one was '/'
        while(ch != '\n' && ch != EOF) // loop through until you find the end of the line or the end of the file
        {
            spelling += ch; // add on the current character to spelling
            nextch(); // find the next character
        }
        return new_token(kind, spelling, start_line, start_column);
    }

    // if it is a symbol (previously defined in next_token()) 
    Token parse_single_symbol (TokenKind kind, string spelling)
    {
        nextch();// push the next ch for next next_token()
        if(kind == tk_div) // checking if we have a '/' for comments
        {   
            if (ch == '/') return parse_eol_com(tk_eol_comment, ""); //checking if we have a //
            else if (ch == '*') return parse_adhoc_com(tk_adhoc_comment, ""); //checking if we have a /*
        }
        return new_token (kind, spelling, start_line, start_column); // push the variables into a token
    }

    Token parse_eq(TokenKind kind, string spelling) // parses the equals symbol
    {
        nextch(); // as the previous value was =
        if (ch == '=') // if the next value is equals, we're good
        {
            spelling += ch; // this should be ==
            return new_token (kind, spelling, start_line, start_column); // and == is the correct spelling for tk_eq
        }
        else return parse_eoi(); // if its not ==, then its bad syntax
    }
    
    Token parse_spaceship(TokenKind kind, string spelling) // parses the spaceship symbol
    {
        nextch(); // to move ch to the = sign
        if (ch == '=') // the next character in spaceship after '<' (which is the value currently inside spelling)
        {
            spelling += ch;
            nextch(); // to move ch to the > sign
            if (ch == '>')
            {
                spelling += ch; // the last character required to make a spaceship
                return new_token (kind, spelling, start_line, start_column); 
            }
        }
        else return parse_eoi(); // if its not equals, its bad syntax
    }

    Token parse_multi_symbol (TokenKind kind, string spelling) // parses symbol that is mulitple character long
    {
        if (spelling == "<") // as the only multi-character symbol that starts with <, this can only be a space ship
        {
            return parse_spaceship(tk_spaceship, spelling);
        }
        else if (spelling == "=")// as the only multi-character symbol that starts with =, this can only be a tk_eq
        {
            return parse_eq(tk_eq, spelling);
        }
        else return parse_eoi(); // just in case something triggers this.
    }

    // return the next token object by reading more of the input
    Token next_token()
    {
        // you must read input using the nextch() function
        // the last character read is in the static variable ch
        // always read one character past the end of the token being returned

        string spelling = "" ;

        // this loop reads one character at a time until it reaches end of input
        while ( ch != EOF )
        {
            start_line = line_num ;                 // remember current position in case we find a token
            start_column = column_num ;

            switch(ch)                              // ch is always the next char to read
            {
            case ' ':                               // ignore space and newline
            case '\n':
                nextch() ;                          // read one more character and try again
                break ;
                                                // add additional case labels here for characters that can start tokens
                                                // call a parse_*(kind,spelling) function to complete and return each kind of token
                                                // this should follow the style used in the workshops
                                                // but remember that the token grammar is different in this assignment
                case'_':
                case 'A'...'Z':
                case 'a'...'z': return parse_identifier (tk_identifier, spelling); // if the first character is a letter, then make identifier
                
                case '1'...'9': return parse_integer (tk_integer, spelling); // if the first character is an integer, then the pin it to an integer
                case '0': return parse_zero_integer (tk_integer, spelling); // if the first character is zero, then it is zero

                // if the first character is a symbol, then pin it to a token, and continue
                case '@': return parse_single_symbol(tk_at, "@"); 
                case ';': return parse_single_symbol(tk_semi, ";");  
                case ':': return parse_single_symbol(tk_colon, ":"); 
                case '!': return parse_single_symbol(tk_not, "!"); 
                case ',': return parse_single_symbol(tk_comma, ","); 
                case '.': return parse_single_symbol(tk_stop, "."); 
                case '{': return parse_single_symbol(tk_lcb, "{"); 
                case '}': return parse_single_symbol(tk_rcb, "}"); 
                case '(': return parse_single_symbol(tk_lrb, "("); 
                case ')': return parse_single_symbol(tk_rrb, ")");
                case '[': return parse_single_symbol(tk_lsb, "[");
                case ']': return parse_single_symbol(tk_rsb, "]");
                case '/': return parse_single_symbol(tk_div, "/");
                case '=': return parse_multi_symbol(tk_eq, "=");
                case '<': return parse_multi_symbol(tk_spaceship, "<");
                case '"': return parse_string(tk_string,"");
                
                default:
                return parse_eoi() ;                // the next character cannot start a token, this is an error, return an EOI token
            }
        }

        start_line = line_num ;                     // remember current position so EOI token is correct
        start_column = column_num ;
        Token next_token() ;
        return parse_eoi() ;                         // return an EOI token
    }
}
