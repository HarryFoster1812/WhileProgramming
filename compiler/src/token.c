#include "token.h"
#include <stdlib.h>
#include <string.h>

const char* TOKEN_TYPE_MAP[] = {
    "TOKEN_VARIABLE", "TOKEN_NUMBER",  "TOKEN_ASSIGNMENT", "TOKEN_SEMICOLON",
    "TOKEN_WHILE",    "TOKEN_DO",      "TOKEN_IF",         "TOKEN_THEN",
    "TOKEN_ELSE",     "TOKEN_INPUT",   "TOKEN_PRINT",      "TOKEN_L_CURLY",
    "TOKEN_R_CURLY",  "TOKEN_L_PAREN", "TOKEN_R_PAREN",    "TOKEN_PLUS",
    "TOKEN_MINUS",    "TOKEN_LEQ",     "TOKEN_EQUAL",      "TOKEN_NOT",
    "TOKEN_AND",      "TOKEN_TRUE",    "TOKEN_FALSE",      "TOKEN_SKIP",
    "TOKEN_EOF",      "TOKEN_UNKNOWN"};

const TokenMap keywords[] = {
    {"while", TOKEN_WHILE}, {"do", TOKEN_DO},         {"if", TOKEN_IF},
    {"then", TOKEN_THEN},   {"else", TOKEN_ELSE},     {"input", TOKEN_INPUT},
    {"print", TOKEN_PRINT}, {":=", TOKEN_ASSIGNMENT}, {"<=", TOKEN_LEQ},
    {"==", TOKEN_EQUAL},    {";", TOKEN_SEMICOLON},   {"+", TOKEN_PLUS},
    {"-", TOKEN_MINUS},     {"{", TOKEN_L_CURLY},     {"}", TOKEN_R_CURLY},
    {"(", TOKEN_L_PAREN},   {")", TOKEN_R_PAREN},     {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE}, {"skip", TOKEN_SKIP},     {"&&", TOKEN_AND},
    {"!", TOKEN_NOT}};

const int keywords_count = sizeof(keywords) / sizeof(TokenMap);

TOKEN_T* make_token(enum TOKEN_TYPE type, int line, int col, char* lexeme) {
    TOKEN_T* ptr = (TOKEN_T*)malloc(sizeof(TOKEN_T));
    if (ptr) {
        ptr->type = type;
        ptr->line_number = line;
        ptr->col_number = col;
        ptr->lexeme = lexeme; // string passed must be heap allocated
    }
    return ptr;
}

int is_symbol(char c) { return strchr("+-;{}():=<>&!/", c) != NULL; }

int is_multi_character_symbol(char* buffer, int* col_no) {
    if ((buffer[*col_no] == ':' && buffer[*col_no + 1] == '=') ||
        (buffer[*col_no] == '<' && buffer[*col_no + 1] == '=') ||
        (buffer[*col_no] == '&' && buffer[*col_no + 1] == '&') ||
        (buffer[*col_no] == '/' && buffer[*col_no + 1] == '/') ||
        (buffer[*col_no] == '=' && buffer[*col_no + 1] == '=')) {
        *col_no += 2;
        return 1;
    }
    return 0;
}

void free_token(TOKEN_T* token) {
    if (token) {
        if (token->lexeme) {
            free(token->lexeme);
        }
        free(token);
    }
}
