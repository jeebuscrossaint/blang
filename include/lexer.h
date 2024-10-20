#ifndef LEXER_H
#define LEXER_H

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

//types of tokens we will lex

// is that a word?
typedef enum {
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