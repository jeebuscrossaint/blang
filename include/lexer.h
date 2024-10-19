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
    TOKEN_EOF,         // End of file
    TOKEN_IDENTIFIER,  // Variable/function names
    TOKEN_NUMBER,      // Numeric literals
    TOKEN_STRING,      // String literals
    TOKEN_KEYWORD,     // Keywords like if, while, return, etc.
    TOKEN_OPERATOR,    // Operators like +, -, *, /, =, etc.
    TOKEN_PUNCTUATION, // Symbols like (, ), {, }, ;, etc.
    TOKEN_ERROR        // Error or unrecognized token
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