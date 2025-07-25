#pragma once

enum TOKEN_TYPE {
  TOKEN_VARIABLE,
  TOKEN_NUMBER,
  TOKEN_ASSIGNMENT,
  TOKEN_SEMICOLON,
  TOKEN_WHILE,
  TOKEN_DO,
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_INPUT,
  TOKEN_PRINT,
  TOKEN_L_CURLY,
  TOKEN_R_CURLY,
  TOKEN_L_PAREN,
  TOKEN_R_PAREN,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_LEQ,
  TOKEN_EQUAL,
  TOKEN_NOT,
  TOKEN_AND,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_SKIP,
  TOKEN_EOF,
  TOKEN_UNKNOWN // optional, for error recovery
};

// Declare the token type strings as extern, defined in token.c
extern const char *TOKEN_TYPE_MAP[];

// Token keyword map - extern declaration
typedef struct {
  const char *lexeme;
  enum TOKEN_TYPE type;
} TokenMap;

extern const TokenMap keywords[];
extern const int keywords_count;

// Token struct
typedef struct {
  enum TOKEN_TYPE type;
  int line_number;
  int col_number;
  char *lexeme;
} TOKEN_T;

// Function prototypes
TOKEN_T *make_token(enum TOKEN_TYPE type, int line, int col, char *lexeme);

int is_symbol(char c);

int is_multi_character_symbol(char *buffer, int *col_no);

void free_token(TOKEN_T *token);
