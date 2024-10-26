#ifndef LEXER_H
#define LEXER_H

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

//types of tokens we will lex

// is that a word?
/*typedef enum {
    TOKEN_EOF,         // End of file type 0
    TOKEN_IDENTIFIER,  // Variable/function names type 1
    TOKEN_NUMBER,      // Numeric literals type 2
    TOKEN_STRING,      // String literals type 3
    TOKEN_KEYWORD,     // Keywords like if, while, return, etc. type 4
    TOKEN_OPERATOR,    // Operators like +, -, *, /, =, etc. type 5
    TOKEN_PUNCTUATION, // Symbols like (, ), {, }, ;, etc. type 6
    TOKEN_COMMENT,     // Comments type 7
    TOKEN_WHITESPACE,  // Whitespace type 8
    TOKEN_ERROR        // Error or unrecognized token type 9
} TokenType; */

typedef enum {
    /* language perks */
    TOKEN_SOURCE, // Source another file like how you do in shell scripts because #include is lame
    TOKEN_ASSEMBLY, // inline assembly?
    TOKEN_RETURN, // return keyword
    TOKEN_SIZEOF, // sizeof unary operator
    TOKEN_TERM, // ; to terminate lines
    TOKEN_EOF, // end of file i think
    TOKEN_ERROR, // unknown type
    /* comparisons and comments */
    TOKEN_LTG, // less than greater than sign <
    TOKEN_GTL, // greater than less than
    TOKEN_LTGEQ, // less than greater than equal to sign =<
    TOKEN_GTLEQ, // greater than equal to sign >=
    TOKEN_SINGLCOMMENT, // single line comments // yk the drill
    TOKEN_MULTICOMMENT, // multiline comments /* yk the drirll */
    TOKEN_ASSEQUAL, // equal value assigner x = y+12
    TOKEN_VALEQUAL, // Equal value checker probably should be in the boolean section but whatever LOL
    TOKEN_QUESTION, // question mark boolean algebra ?
    TOKEN_ANDAND, // && operation
    TOKEN_OROR, // || operation
    TOKEN_TRUE, // true literal
    TOKEN_FALSE, // false literal
    TOKEN_NOTEQUAL,// ! not
    /* mathematical data types */
    TOKEN_AUTO, // auto data type we all know and love
    TOKEN_INT, // specifiy integer
    TOKEN_UINT, // unsigned integer
    TOKEN_CONST, // const stuff
    TOKEN_DOUBLE, // double data type
    TOKEN_EXTERN, // externalize a data type
    TOKEN_LONG, // long data type
    TOKEN_LONGLONG, // two longs idk what tf to tell u dawg
    TOKEN_SHORT, // i guess this is a short, like me
    TOKEN_SHORTSHORT, // ngl a meme type lol
    TOKEN_FLOAT, // float type yk decimal points
    /* linguistic types */
    TOKEN_STRING, // strings
    TOKEN_CHAR, // a character
    TOKEN_ENUM, // enums cuz they are epic and i am writing one rn for this lexer
    TOKEN_STRUCT, // structs
    TOKEN_UNION, // this will be wierd to implement but ok
    TOKEN_ARRAY, // ARRAYS!!
    TOKEN_VECTOR, // its ok if i dont implemenet this ebcause this seems jank to write i mean have you SEEN the C++ vector souce  code and header file?
    /* logic */
    TOKEN_BREAK, // break logic 
    TOKEN_WHILE, // while loop
    TOKEN_FOR, // for loop
    TOKEN_CASE, // cases
    TOKEN_DO, // do portion
    TOKEN_ELSE, // Else statement
    TOKEN_IF, // if statement
    TOKEN_SWITCH, // switch statements
    TOKEN_ELIF, // Elif keyword
    TOKEN_DEFAULT, // switch case default
    /* computation ( really just constants )*/
    TOKEN_PI, // pi constant 3.14
    TOKEN_GOLDEN, // golden ratio 1+sqrt5 / 2
    TOKEN_SILVER, // silver ratio sqrt2 + 1
    TOKEN_IMAGINE, // imaginary number sqrt-1
    TOKEN_EULER, // euler number 2.78
} TokenType;



// Struct to represent a token
typedef struct {
    TokenType type; // Type of token
    char* value;    // Actual value of the token
    int line;       // Line number
    int column;     // Column number
} Token;

// Struct to represent the lexer
typedef struct {
    const char* input; // The entire source code as a string
    size_t pos;        // Current position in the string
    int line;          // Current line
    int column;        // Current column
    Token* tokens;     // Dynamic array of tokens
    size_t token_count;// Number of tokens
    size_t token_capacity; // Capacity of the token array
} Lexer;

// Function prototypes
void init_lexer(Lexer* lexer, const char* input);
Token get_next_token(Lexer* lexer);
void process_file(const char* filename);
void cleanup_lexer(Lexer* lexer);

#endif // LEXER_H