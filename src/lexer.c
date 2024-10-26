#include "../include/lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void ensure_token_capacity(Lexer* lexer) {
    if (lexer->token_count >= lexer->token_capacity) {
        lexer->token_capacity *= 2;
        lexer->tokens = (Token*)realloc(lexer->tokens, lexer->token_capacity * sizeof(Token));
        if (!lexer->tokens) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
    }
}

void init_lexer(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
    lexer->token_capacity = 10;
    lexer->tokens = (Token*)malloc(lexer->token_capacity * sizeof(Token));
    if (!lexer->tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

Token create_token(Lexer* lexer, TokenType type, size_t start, size_t length) {
    Token token;
    token.type = type;
    token.value = strndup(lexer->input + start, length);
    if (!token.value && length > 0) {
        fprintf(stderr, "Memory allocation failed for token value\n");
        exit(1);
    }
    token.line = lexer->line;
    token.column = lexer->column - length;
    return token;
}

void add_token(Lexer* lexer, Token token) {
    ensure_token_capacity(lexer);
    lexer->tokens[lexer->token_count++] = token;
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->input[lexer->pos]) {
        char c = lexer->input[lexer->pos];
        if (c == ' ' || c == '\t') {
            lexer->column++;
        } else if (c == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else if (c == '\r') {
            // Skip carriage return
        } else {
            break;
        }
        lexer->pos++;
    }
}

Token lex_identifier_or_keyword(Lexer* lexer) {
    size_t start = lexer->pos;
    
    // First character must be letter or underscore
    if (!isalpha(lexer->input[lexer->pos]) && lexer->input[lexer->pos] != '_') {
        return create_token(lexer, TOKEN_ERROR, start, 1);
    }

    // Read all alphanumeric characters or underscores
    while (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_') {
        lexer->pos++;
        lexer->column++;
    }
    
    size_t length = lexer->pos - start;
    char* value = strndup(lexer->input + start, length);
    TokenType type = TOKEN_IDENTIFIER;

    // Check for keywords
    struct {
        const char* keyword;
        TokenType type;
    } keywords[] = {
        {"source", TOKEN_SOURCE},
        {"asm", TOKEN_ASSEMBLY},
        {"return", TOKEN_RETURN},
        {"sizeof", TOKEN_SIZEOF},
        {"auto", TOKEN_AUTO},
        {"int", TOKEN_INT},
        {"uint", TOKEN_UINT},
        {"const", TOKEN_CONST},
        {"double", TOKEN_DOUBLE},
        {"extern", TOKEN_EXTERN},
        {"long", TOKEN_LONG},
        {"longlong", TOKEN_LONGLONG},
        {"short", TOKEN_SHORT},
        {"shortshort", TOKEN_SHORTSHORT},
        {"float", TOKEN_FLOAT},
        {"string", TOKEN_STRING},
        {"char", TOKEN_CHAR},
        {"enum", TOKEN_ENUM},
        {"struct", TOKEN_STRUCT},
        {"union", TOKEN_UNION},
        {"array", TOKEN_ARRAY},
        {"vector", TOKEN_VECTOR},
        {"break", TOKEN_BREAK},
        {"while", TOKEN_WHILE},
        {"for", TOKEN_FOR},
        {"case", TOKEN_CASE},
        {"do", TOKEN_DO},
        {"else", TOKEN_ELSE},
        {"if", TOKEN_IF},
        {"switch", TOKEN_SWITCH},
        {"elif", TOKEN_ELIF},
        {"default", TOKEN_DEFAULT},
        {"cpi", TOKEN_PI},
        {"rgolden", TOKEN_GOLDEN},
        {"rsilver", TOKEN_SILVER},
        {"cimagine", TOKEN_IMAGINE},
        {"ceuler", TOKEN_EULER},
        {"true", TOKEN_TRUE},
        {"false", TOKEN_FALSE},
        {NULL, TOKEN_IDENTIFIER}  // sentinel value
    };

    for (int i = 0; keywords[i].keyword != NULL; i++) {
        if (strcmp(value, keywords[i].keyword) == 0) {
            type = keywords[i].type;
            break;
        }
    }

    free(value);  // Free the temporary value
    return create_token(lexer, type, start, length);
}

Token lex_number(Lexer* lexer) {
    size_t start = lexer->pos;
    bool is_float = false;
    
    // Check for hexadecimal
    if (lexer->input[lexer->pos] == '0' && 
        (lexer->input[lexer->pos + 1] == 'x' || lexer->input[lexer->pos + 1] == 'X')) {
            
        lexer->pos += 2;  // Skip "0x" or "0X"
        lexer->column += 2;
        
        // Make sure we have at least one hex digit
        if (!isxdigit(lexer->input[lexer->pos])) {
            return create_token(lexer, TOKEN_ERROR, start, lexer->pos - start);
        }
        
        // Read all hex digits
        while (isxdigit(lexer->input[lexer->pos])) {
            lexer->pos++;
            lexer->column++;
        }
        
        return create_token(lexer, TOKEN_HEX_NUMBER, start, lexer->pos - start);
    }
    
    // Handle regular decimal numbers
    while (isdigit(lexer->input[lexer->pos])) {
        lexer->pos++;
        lexer->column++;
    }
    
    // Check for decimal point
    if (lexer->input[lexer->pos] == '.') {
        lexer->pos++;
        lexer->column++;
        is_float = true;
        
        // Read decimal places
        while (isdigit(lexer->input[lexer->pos])) {
            lexer->pos++;
            lexer->column++;
        }
    }
    
    // Look for exponent marker
    if (lexer->input[lexer->pos] == 'e' || lexer->input[lexer->pos] == 'E') {
        is_float = true;
        lexer->pos++;
        lexer->column++;
        
        // Optional + or - after exponent marker
        if (lexer->input[lexer->pos] == '+' || lexer->input[lexer->pos] == '-') {
            lexer->pos++;
            lexer->column++;
        }
        
        // Must have at least one digit after exponent
        if (!isdigit(lexer->input[lexer->pos])) {
            return create_token(lexer, TOKEN_ERROR, start, lexer->pos - start);
        }
        
        while (isdigit(lexer->input[lexer->pos])) {
            lexer->pos++;
            lexer->column++;
        }
    }
    
    // Decide final token type
    TokenType type;
    if (is_float) {
        type = TOKEN_FLOAT;
    } else {
        type = TOKEN_NUMBER;
    }
    
    return create_token(lexer, type, start, lexer->pos - start);
}

Token lex_punctuation(Lexer* lexer) {
    size_t start = lexer->pos;
    TokenType type;
    
    switch (lexer->input[lexer->pos]) {
        case '(': type = TOKEN_LPAREN; break;
        case ')': type = TOKEN_RPAREN; break;
        case '{': type = TOKEN_LBRACE; break;
        case '}': type = TOKEN_RBRACE; break;
        case '[': type = TOKEN_LBRACKET; break;
        case ']': type = TOKEN_RBRACKET; break;
        case ';': type = TOKEN_SEMICOLON; break;
        case ',': type = TOKEN_COMMA; break;
        case '.': type = TOKEN_DOT; break;
        default: type = TOKEN_ERROR;
    }
    
    lexer->pos++;
    lexer->column++;
    return create_token(lexer, type, start, 1);
}

Token lex_string(Lexer* lexer) {
    lexer->pos++; // Skip opening quote
    lexer->column++;
    size_t start = lexer->pos;
    
    while (lexer->input[lexer->pos] != '"' && lexer->input[lexer->pos] != '\0') {
        if (lexer->input[lexer->pos] == '\\') {
            // Handle escape sequences
            lexer->pos++;
            lexer->column++;
            if (lexer->input[lexer->pos] == '\0') {
                // Error: unexpected end of file
                return create_token(lexer, TOKEN_ERROR, start, lexer->pos - start);
            }
        }
        if (lexer->input[lexer->pos] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->pos++;
    }
    
    if (lexer->input[lexer->pos] == '\0') {
        // Error: unterminated string
        return create_token(lexer, TOKEN_ERROR, start - 1, lexer->pos - start + 1);
    }
    
    size_t length = lexer->pos - start;
    lexer->pos++; // Skip closing quote
    lexer->column++;
    return create_token(lexer, TOKEN_STRING, start, length);
}

Token lex_operator(Lexer* lexer) {
    size_t start = lexer->pos;
    char first = lexer->input[lexer->pos];
    char next = lexer->input[lexer->pos + 1];
    TokenType type;
    size_t length = 1;
    
    // Handle two-character operators
    if (next == '=') {
        switch (first) {
            case '=': type = TOKEN_VALEQUAL; length = 2; break;
            case '<': type = TOKEN_LTGEQ; length = 2; break;
            case '>': type = TOKEN_GTLEQ; length = 2; break;
            case '!': type = TOKEN_NOTEQUAL; length = 2; break;
            case '+': type = TOKEN_PLUSEQUAL; length = 2; break;
            case '-': type = TOKEN_MINUSEQUAL; length = 2; break;
            case '*': type = TOKEN_MULEQUAL; length = 2; break;
            case '/': type = TOKEN_DIVEQUAL; length = 2; break;
            default: type = TOKEN_OPERATOR;
        }
    } else {
        // Single-character operators
        switch (first) {
            case '+': type = TOKEN_PLUS; break;
            case '-': type = TOKEN_MINUS; break;
            case '*': type = TOKEN_MULTIPLY; break;
            case '/': type = TOKEN_DIVIDE; break;
            case '<': type = TOKEN_LTG; break;
            case '>': type = TOKEN_GTL; break;
            case '=': type = TOKEN_ASSEQUAL; break;
            case '!': type = TOKEN_NOT; break;
            default: type = TOKEN_OPERATOR;
        }
    }
    
    lexer->pos += length;
    lexer->column += length;
    return create_token(lexer, type, start, length);
}

Token get_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->pos] == '\0') {
        return create_token(lexer, TOKEN_EOF, lexer->pos, 0);
    }
    
    Token token;
    
    // Handle comments first
    if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '/') {
        size_t start = lexer->pos + 2;
        lexer->pos += 2;
        lexer->column += 2;
        while (lexer->input[lexer->pos] != '\n' && lexer->input[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
        token = create_token(lexer, TOKEN_SINGLCOMMENT, start, lexer->pos - start);
    }
    else if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '*') {
        size_t start = lexer->pos + 2;
        lexer->pos += 2;
        lexer->column += 2;
        while (!(lexer->input[lexer->pos] == '*' && lexer->input[lexer->pos + 1] == '/')) {
            if (lexer->input[lexer->pos] == '\0') {
                return create_token(lexer, TOKEN_ERROR, start - 2, lexer->pos - start + 2);
            }
            if (lexer->input[lexer->pos] == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->pos++;
        }
        token = create_token(lexer, TOKEN_MULTICOMMENT, start, lexer->pos - start);
        lexer->pos += 2; // Skip */
        lexer->column += 2;
    }
    else if (isalpha(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_') {
        token = lex_identifier_or_keyword(lexer);
    }
    else if (isdigit(lexer->input[lexer->pos])) {
        token = lex_number(lexer);
    }
    else if (lexer->input[lexer->pos] == '"') {
        token = lex_string(lexer);
    }
    else if (strchr("+-*/=<>!", lexer->input[lexer->pos])) {
        token = lex_operator(lexer);
    }
    else if (strchr("(){}[];,.", lexer->input[lexer->pos])) {
        token = lex_punctuation(lexer);
    }
    else {
        token = create_token(lexer, TOKEN_ERROR, lexer->pos, 1);
        lexer->pos++;
        lexer->column++;
    }
    
    add_token(lexer, token);
    return token;
}

void process_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return;
    }

    size_t read_size = fread(buffer, 1, length, file);
    buffer[read_size] = '\0';
    fclose(file);

    Lexer lexer;
    init_lexer(&lexer, buffer);

    Token token;
    do {
        token = get_next_token(&lexer);
        printf("Token: Type=%d, Value='%s', Line=%zu, Column=%zu\n",
               token.type,
               token.value ? token.value : "NULL",
               token.line,
               token.column);
    } while (token.type != TOKEN_EOF);

    cleanup_lexer(&lexer);
    free(buffer);
}

void cleanup_lexer(Lexer* lexer) {
    for (size_t i = 0; i < lexer->token_count; i++) {
        free(lexer->tokens[i].value);
    }
    free(lexer->tokens);
}