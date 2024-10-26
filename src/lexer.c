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

// logic, keyword or identifier parsing
Token lex_identifier_or_keyword(Lexer* lexer) {
    size_t start = lexer->pos;

    // Check for single-line comment
    if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '/') {
        lexer->pos += 2;
        lexer->column += 2;
        start = lexer->pos;
        while (lexer->input[lexer->pos] != '\n' && lexer->input[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        return create_token(lexer, TOKEN_SINGLCOMMENT, start, length);
    }

    // Check for multi-line comment
    if (lexer->input[lexer->pos] == '/' && lexer->input[lexer->pos + 1] == '*') {
        lexer->pos += 2;
        lexer->column += 2;
        start = lexer->pos;
        while (!(lexer->input[lexer->pos] == '*' && lexer->input[lexer->pos + 1] == '/') && lexer->input[lexer->pos] != '\0') {
            lexer->pos++;
            lexer->column++;
        }
        size_t length = lexer->pos - start;
        lexer->pos += 2; // Skip closing */
        lexer->column += 2;
        return create_token(lexer, TOKEN_MULTICOMMENT, start, length);
    }

    // Lex identifiers or keywords
    while (isalnum(lexer->input[lexer->pos])) {
        lexer->pos++;
        lexer->column++;
    }
    size_t length = lexer->pos - start;
    char* value = strndup(lexer->input + start, length);

    TokenType type = TOKEN_IDENTIFIER;
    // language perks
    if (strcmp(value, "source") == 0) {
        type = TOKEN_SOURCE;
    } else if (strcmp(value, "asm") == 0) {
        type = TOKEN_ASSEMBLY;
    } else if (strcmp(value, "return") == 0) {
        type = TOKEN_RETURN;
    } else if (strcmp(value, "sizeof") == 0) {
        type = TOKEN_SIZEOF;
    } else if (strcmp(value, ";") == 0) {
        type = TOKEN_TERMINATE;
    } else if (strcmp(value, "SOF") == 0) {
        type = TOKEN_SOF;
    } else if (strcmp(value, "EOF") == 0) {
        type = TOKEN_EOF;
    } // primitive types 
    else if (strcmp(value, "auto") == 0) {
        type = TOKEN_AUTO;
    } else if (strcmp(value, "int") == 0) {
        type = TOKEN_INT;
    } else if (strcmp(value, "uint") == 0) {
        type = TOKEN_UINT;
    } else if (strcmp(value, "const") == 0) {
        type = TOKEN_CONST;
    } else if (strcmp(value, "double") == 0) {
        type = TOKEN_DOUBLE;
    } else if (strcmp(value, "extern") == 0) {
        type = TOKEN_EXTERN;
    } else if (strcmp(value, "long") == 0) {
        type = TOKEN_LONG;
    } else if (strcmp(value, "longlong") == 0) {
        type = TOKEN_LONGLONG;
    } else if (strcmp(value, "short") == 0) {
        type = TOKEN_SHORT;
    } else if (strcmp(value, "shortshort") == 0) {
        type = TOKEN_SHORTSHORT;
    } else if (strcmp(value, "float") == 0) {
        type = TOKEN_FLOAT;
    } // comparisons
    else if (strcmp(value, "<") == 0) {
        type = TOKEN_LTG;
    } else if (strcmp(value, ">") == 0) {
        type = TOKEN_GTL;
    } else if (strcmp(value, "=<") == 0) {
        type = TOKEN_LTGEQ;
    } else if (strcmp(value, ">=") == 0) {
        type = TOKEN_GTLEQ;
    } else if (strcmp(value, "=") == 0) {
        type = TOKEN_ASSEQUAL;
    } else if (strcmp(value, "==") == 0) {
        type = TOKEN_VALEQUAL;
    } else if (strcmp(value, "?") == 0) {
        type = TOKEN_QUESTION;
    } else if (strcmp(value, "&&") == 0) {
        type = TOKEN_ANDAND;
    } else if (strcmp(value, "||") == 0) {
        type = TOKEN_OROR;
    } else if (strcmp(value, "true") == 0) {
        type = TOKEN_TRUE;
    } else if (strcmp(value, "false") == 0) {
        type = TOKEN_FALSE;
    } else if (strcmp(value, "!") == 0) {
        type = TOKEN_NOTEQUAL;
    } // linguistic types
    else if (strcmp(value, "string") == 0) {
        type = TOKEN_STRING;
    } else if (strcmp(value, "char") == 0) {
        type = TOKEN_CHAR;
    } else if (strcmp(value, "enum") == 0) {
        type = TOKEN_ENUM;
    } else if (strcmp(value, "struct") == 0) {
        type = TOKEN_STRUCT;
    } else if (strcmp(value, "union") == 0) {
        type = TOKEN_UNION;
    } else if (strcmp(value, "array") == 0) {
        type = TOKEN_ARRAY;
    } else if (strcmp(value, "vector") == 0) {
        type = TOKEN_VECTOR;
    } // recursion
    else if (strcmp(value, "break") == 0) {
        type = TOKEN_BREAK;
    } else if (strcmp(value, "while") == 0) {
        type = TOKEN_WHILE;
    } else if (strcmp(value, "for") == 0) {
        type = TOKEN_FOR;
    } else if (strcmp(value, "case") == 0) {
        type = TOKEN_CASE;
    } else if (strcmp(value, "do") == 0) {
        type = TOKEN_DO;
    } else if (strcmp(value, "else") == 0) {
        type = TOKEN_ELSE;
    } else if (strcmp(value, "if") == 0) {
        type = TOKEN_IF;
    } else if (strcmp(value, "switch") == 0) {
        type = TOKEN_SWITCH;
    } else if (strcmp(value, "elif") == 0) {
        type = TOKEN_ELIF;
    } else if (strcmp(value , "default") == 0) {
        type = TOKEN_DEFAULT;
    } // math constants that may be useful
    else if (strcmp(value, "cpi") == 0) {
        type = TOKEN_PI;
    } else if (strcmp(value, "rgolden") == 0) {
        type = TOKEN_GOLDEN;
    } else if (strcmp(value, "rsilver") == 0) {
        type = TOKEN_SILVER;
    } else if (strcmp(value, "cimagine") == 0) {
        type = TOKEN_IMAGINE;
    } else if (strcmp(value, "ceuler") == 0) {
        type = TOKEN_EULER;
    }

    Token token = create_token(lexer, type, start, length);
    free(value);
    return token;
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
    if (isalpha(lexer->input[lexer->pos]) || (lexer->input[lexer->pos] == '/' && (lexer->input[lexer->pos + 1] == '/' || lexer->input[lexer->pos + 1] == '*'))) {
        return lex_identifier_or_keyword(lexer);
    } else if (isdigit(lexer->input[lexer->pos])) {
        return lex_number(lexer);
    } else if (lexer->input[lexer->pos] == '"') {
        return lex_string(lexer);
    } else if (strchr("+-*/=<>!", lexer->input[lexer->pos])) {
        return lex_operator(lexer);
    } else if (strchr("(){}[];,", lexer->input[lexer->pos])) {
        return lex_punctuation(lexer);
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