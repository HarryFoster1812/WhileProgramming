#pragma once

#include <stdio.h>
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
  TOKEN_BEGIN,
  TOKEN_END,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_LEQ,
  TOKEN_EQUAL,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_EOF,
  TOKEN_UNKNOWN // optional, for error recovery
};

typedef struct {
  enum TOKEN_TYPE type;
  int line_number;
  int col_number;
  char *lexeme;
} TOKEN_T;

TOKEN_T *make_token(enum TOKEN_TYPE type, int line, int col,
                    const char *lexeme);

TOKEN_T **lex_file(FILE *fptr);

#include "./lexer.c"
