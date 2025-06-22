#pragma once
#include "./lexer.h"
#include "util.c"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TOKEN_T *make_token(enum TOKEN_TYPE type, int line, int col, char *lexeme) {
  TOKEN_T *ptr = (TOKEN_T *)malloc(sizeof(TOKEN_T));
  if (ptr) {
    ptr->type = type;
    ptr->line_number = line;
    ptr->col_number = col;
    ptr->lexeme = lexeme; // string passed must be allocated in the heap
  }
  return ptr;
}

typedef struct {
  const char *lexeme;
  enum TOKEN_TYPE type;
} TokenMap;

enum TOKEN_TYPE identify_token(const char *lexeme) {
  static const TokenMap keywords[] = {
      {"while", TOKEN_WHILE}, {"do", TOKEN_DO},         {"if", TOKEN_IF},
      {"then", TOKEN_THEN},   {"else", TOKEN_ELSE},     {"read", TOKEN_INPUT},
      {"print", TOKEN_WRITE}, {":=", TOKEN_ASSIGNMENT}, {"<=", TOKEN_LEQ},
      {"==", TOKEN_EQUAL},    {";", TOKEN_SEMICOLON},   {"+", TOKEN_PLUS},
      {"-", TOKEN_MINUS},     {"{", TOKEN_L_CURLY},     {"}", TOKEN_R_CURLY},
      {"(", TOKEN_L_PAREN},   {")", TOKEN_L_PAREN},     {"true", TOKEN_TRUE},
      {"false", TOKEN_FALSE}, {"skip", TOKEN_SKIP}};

  const int keyword_count = sizeof(keywords) / sizeof(TokenMap);
  for (int i = 0; i < keyword_count; ++i) {
    if (strcmp(lexeme, keywords[i].lexeme) == 0) {
      return keywords[i].type;
    }
  }

  if (isdigit(lexeme[0]))
    return TOKEN_NUMBER;
  if (isalpha(lexeme[0]))
    return TOKEN_VARIABLE;
  return TOKEN_UNKNOWN;
}

int is_symbol(char c) { return strchr("+-;{}():=<>", c) != NULL; }

int is_multi_character_symbol(char *buffer, int *col_no) {
  // this contains the checks for multi characters such as :=, <=, >=
  if ((buffer[*col_no] == ':' && buffer[*col_no + 1] == '=') ||
      (buffer[*col_no] == '<' && buffer[*col_no + 1] == '=') ||
      (buffer[*col_no] == '>' && buffer[*col_no + 1] == '=') ||
      (buffer[*col_no] == '=' && buffer[*col_no + 1] == '=')) {
    *col_no += 2;
    return 1; // true
  }
  return 0; // false
}

TOKEN_T **lex_file(FILE *fptr) {
  // file buffer
  long token_count = 0;
  long token_arr_size = 2000;
  int file_buffer_size = 2048;
  TOKEN_T **token_arr = (TOKEN_T **)malloc(sizeof(TOKEN_T *) * token_arr_size);
  char fbuffer[file_buffer_size];
  int line_no = 1;
  while (fgets(fbuffer, file_buffer_size, fptr)) {
    // for each line in the file
    int col_no = 0;
    while (fbuffer[col_no] != '\0') {

      while (isspace(fbuffer[col_no]))
        col_no++;

      if (fbuffer[col_no] == '\n' || fbuffer[col_no] == '\0')
        break;

      // we are now at the start of a token

      int col_start = col_no; // this is used to index tokens
                              //
      if (isalpha(fbuffer[col_no])) {
        // Identifier or keyword
        while (isalnum(fbuffer[col_no]))
          col_no++;
      } else if (isdigit(fbuffer[col_no])) {
        // Number
        while (isdigit(fbuffer[col_no]))
          col_no++;
      } else if (is_symbol(fbuffer[col_no])) {
        // Try to match multi-character operators first
        if (is_multi_character_symbol(fbuffer, &col_no)) {

        } else {
          col_no++; // Single-character symbol
        }
      } else {
        // Unknown character, treat as single unknown token
        col_no++;
      }

      char *tok = malloc(col_no - col_start + 1);
      // +2 since we also need to account for the null character

      for (int i = 0; i < col_no - col_start; ++i) {
        tok[i] = fbuffer[col_start + i];
      }
      // null terminate the string for the token
      tok[col_no - col_start + 1] = 0;

      enum TOKEN_TYPE type = identify_token(tok);
      // add tokens into the arr

      if (token_count == token_arr_size) {
        // extend token_arr to fit the new values
        token_arr_size *= 2;
        TOKEN_T **new_arr =
            realloc(token_arr, sizeof(TOKEN_T *) * token_arr_size);
        if (!new_arr) {
          fprintf(stderr, "Memory allocation failed.\n");
          exit(1);
        }
        token_arr = new_arr;
      }

      token_arr[token_count++] = make_token(type, line_no, col_start, tok);
    }
    line_no++;
  }
  token_arr[token_count++] = make_token(TOKEN_EOF, line_no, 0, "EOF");
  // realloc to free any unused Memory
  token_arr = realloc(token_arr, sizeof(TOKEN_T *) * token_count);
  return token_arr;
}
