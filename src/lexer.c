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

Token create_token(Lexer* lexer, TokenType type, size_t start, size_t length) {
    Token token;
    token.type = type;
    token.value = strndup(lexer->input + start, length);
    token.line = lexer->line;
    token.column = lexer->column - length;
    return token;
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->input[lexer->pos] == ' ' || lexer->input[lexer->pos] == '\t' || lexer->input[lexer->pos] == '\n') {
        if (lexer->input[lexer->pos] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->pos++;
    }
}


Token lex_identifier_or_keyword(Lexer* lexer) {
    size_t start = lexer->pos;
    while (isalnum(lexer->input[lexer->pos])) {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    char* value = strndup(lexer->input + start, length);

    TokenType type = TOKEN_IDENTIFIER;
    if (strcmp(value, "if") == 0 || strcmp(value, "else") == 0 || 
        strcmp(value, "while") == 0 || strcmp(value, "return") == 0 || 
        strcmp(value, "print") == 0 || strcmp(value, "end") == 0) {
        type = TOKEN_KEYWORD;
    }

    Token token = create_token(lexer, type, start, length);
    free(value);
    return token;
}

Token lex_number(Lexer* lexer) {
    size_t start = lexer->pos;
    while (isdigit(lexer->input[lexer->pos])) {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    return create_token(lexer, TOKEN_NUMBER, start, length);
}

Token lex_string(Lexer* lexer) {
    lexer->pos++; // Skip opening quote
    lexer->column++;
    size_t start = lexer->pos;
    while (lexer->input[lexer->pos] != '"' && lexer->input[lexer->pos] != '\0') {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    lexer->pos++; // Skip closing quote
    lexer->column++;
    return create_token(lexer, TOKEN_STRING, start, length);
}

Token lex_operator(Lexer* lexer) {
    size_t start = lexer->pos;
    lexer->pos++;
    lexer->column++;
    if (lexer->input[lexer->pos] == '=' && strchr("=<>!", lexer->input[start])) {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    return create_token(lexer, TOKEN_OPERATOR, start, length);
}

Token lex_punctuation(Lexer* lexer) {
    size_t start = lexer->pos;
    lexer->pos++;
    lexer->column++;
    return create_token(lexer, TOKEN_PUNCTUATION, start, 1);
}

Token lex_single_line_comment(Lexer* lexer) {
    lexer->pos += 2;
    lexer->column += 2;
    size_t start = lexer->pos;
    while (lexer->input[lexer->pos] != '\n' && lexer->input[lexer->pos] != '\0') {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    return create_token(lexer, TOKEN_COMMENT, start, length);
}

Token lex_multi_line_comment(Lexer* lexer) {
    lexer->pos += 2;
    lexer->column += 2;
    size_t start = lexer->pos;
    while (!(lexer->input[lexer->pos] == '*' && lexer->input[lexer->pos + 1] == '/') && lexer->input[lexer->pos] != '\0') {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    lexer->pos += 2; // Skip closing */
    lexer->column += 2;
    return create_token(lexer, TOKEN_COMMENT, start, length);
}

Token get_next_token(Lexer* lexer) {
    skip_whitespace(lexer);

    Token token;
    token.line = lexer->line;
    token.column = lexer->column;

    // Check for end of input
    if (lexer->input[lexer->pos] == '\0') {
        token.type = TOKEN_EOF;
        token.value = NULL;
        return token;
    }

    // Lex different types of tokens
    if (isalpha(lexer->input[lexer->pos])) {
        return lex_identifier_or_keyword(lexer);
    } else if (isdigit(lexer->input[lexer->pos])) {
        return lex_number(lexer);
    } else if (lexer->input[lexer->pos] == '"') {
        return lex_string(lexer);
    } else if (strchr("+-*/=<>!", lexer->input[lexer->pos])) {
        return lex_operator(lexer);
    } else if (strchr("(){}[];,", lexer->input[lexer->pos])) {
        return lex_punctuation(lexer);
    } else if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '/') {
        return lex_single_line_comment(lexer);
    } else if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '*') {
        return lex_multi_line_comment(lexer);
    } else {
        // If no rule matches, return an error token
        size_t start = lexer->pos;
        lexer->pos++;
        lexer->column++;
        return create_token(lexer, TOKEN_ERROR, start, 1);
    }
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
        // No need to free t.value here, it will be freed in cleanup_lexer
    }

    cleanup_lexer(&lexer);
    free(buffer);
}

void cleanup_lexer(Lexer* lexer) {
    for (size_t i = 0; i < lexer->token_count; i++) {
        printf("Freeing token value: %s\n", lexer->tokens[i].value);
        free(lexer->tokens[i].value);
    }
    free(lexer->tokens);
}