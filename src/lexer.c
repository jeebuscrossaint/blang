#include "../include/lexer.h"

void init_lexer(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
    lexer->token_capacity = 10; // Initial capacity
    lexer->tokens = (Token*)malloc(lexer->token_capacity * sizeof(Token));
}

Token get_next_token(Lexer* lexer) {
    Token token;
    token.line = lexer->line;
    token.column = lexer->column;

    // Skip whitespace
    while (lexer->input[lexer->pos] == ' ' || lexer->input[lexer->pos] == '\t' || lexer->input[lexer->pos] == '\n') {
        if (lexer->input[lexer->pos] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->pos++;
    }

    // Check for end of input
    if (lexer->input[lexer->pos] == '\0') {
        token.type = TOKEN_EOF;
        token.value = NULL;
        return token;
    }

    // Example: Lexing identifiers (variable names) and keywords
    if (isalpha(lexer->input[lexer->pos])) {
        size_t start = lexer->pos;
        while (isalnum(lexer->input[lexer->pos])) {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        token.value = strndup(lexer->input + start, length);

        // Check if the identifier is a keyword
        if (strcmp(token.value, "if") == 0 || strcmp(token.value, "else") == 0 || 
            strcmp(token.value, "while") == 0 || strcmp(token.value, "return") == 0 || 
            strcmp(token.value, "print") == 0 || strcmp(token.value, "end") == 0) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
    } else if (isdigit(lexer->input[lexer->pos])) {
        // Example: Lexing numeric literals
        size_t start = lexer->pos;
        while (isdigit(lexer->input[lexer->pos])) {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        token.type = TOKEN_NUMBER;
        token.value = strndup(lexer->input + start, length);
    } else if (lexer->input[lexer->pos] == '"') {
        // Example: Lexing string literals
        lexer->pos++;
        lexer->column++;
        size_t start = lexer->pos;
        while (lexer->input[lexer->pos] != '"' && lexer->input[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        token.type = TOKEN_STRING;
        token.value = strndup(lexer->input + start, length);
        lexer->pos++; // Skip closing quote
        lexer->column++;
    } else if (strchr("+-*/=<>!", lexer->input[lexer->pos])) {
        // Example: Lexing operators
        size_t start = lexer->pos;
        lexer->pos++;
        lexer->column++;
        if (lexer->input[lexer->pos] == '=' && strchr("=<>!", lexer->input[start])) {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        token.type = TOKEN_OPERATOR;
        token.value = strndup(lexer->input + start, length);
    } else if (strchr("(){}[];,", lexer->input[lexer->pos])) {
        // Example: Lexing punctuation
        token.type = TOKEN_PUNCTUATION;
        token.value = strndup(lexer->input + lexer->pos, 1);
        lexer->pos++;
        lexer->column++;
    } else if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '/') {
        // Example: Lexing single-line comments
        lexer->pos += 2;
        lexer->column += 2;
        size_t start = lexer->pos;
        while (lexer->input[lexer->pos] != '\n' && lexer->input[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        token.type = TOKEN_COMMENT;
        token.value = strndup(lexer->input + start, length);
    } else if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '*') {
        // Example: Lexing multi-line comments
        lexer->pos += 2;
        lexer->column += 2;
        size_t start = lexer->pos;
        while (!(lexer->input[lexer->pos] == '*' && lexer->input[lexer->pos + 1] == '/') && lexer->input[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        token.type = TOKEN_COMMENT;
        token.value = strndup(lexer->input + start, length);
        lexer->pos += 2; // Skip closing */
        lexer->column += 2;
    } else {
        // If no rule matches, return an error token
        token.type = TOKEN_ERROR;
        token.value = strndup(lexer->input + lexer->pos, 1);
        lexer->pos++;
        lexer->column++;
    }

    // Store the token in the dynamic array
    if (lexer->token_count >= lexer->token_capacity) {
        lexer->token_capacity *= 2;
        lexer->tokens = (Token*)realloc(lexer->tokens, lexer->token_capacity * sizeof(Token));
    }
    lexer->tokens[lexer->token_count++] = token;

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

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    Lexer lexer;
    init_lexer(&lexer, buffer);

    Token token;
    do {
        token = get_next_token(&lexer);
    } while (token.type != TOKEN_EOF);

    // Print tokens for debugging
    for (size_t i = 0; i < lexer.token_count; i++) {
        Token t = lexer.tokens[i];
        printf("Token: Type=%d, Value=%s, Line=%d, Column=%d\n", t.type, t.value, t.line, t.column);
        free(t.value); // Free the token value if it was dynamically allocated
    }

    free(lexer.tokens);
    free(buffer);
}
