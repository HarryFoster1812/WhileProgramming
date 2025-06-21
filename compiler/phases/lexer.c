#pragma once
#include "./lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TOKEN_T *make_token(enum TOKEN_TYPE type, int line, int col,
                    const char *lexeme) {
  TOKEN_T *ptr = (TOKEN_T *)malloc(sizeof(TOKEN_T));
  if (ptr) {
    ptr->type = type;
    ptr->line_number = line;
    ptr->col_number = col;
    ptr->lexeme = strdup(lexeme);
  }
  return ptr;
}

enum TOKEN_TYPE identify_token(const char *lexeme) {
  if (strcmp(lexeme, "while") == 0)
    return TOKEN_WHILE;
  if (strcmp(lexeme, "do") == 0)
    return TOKEN_DO;
  if (strcmp(lexeme, "if") == 0)
    return TOKEN_IF;
  if (strcmp(lexeme, "then") == 0)
    return TOKEN_THEN;
  if (strcmp(lexeme, "else") == 0)
    return TOKEN_ELSE;
  if (strcmp(lexeme, "read") == 0)
    return TOKEN_BEGIN;
  if (strcmp(lexeme, "write") == 0)
    return TOKEN_END;
  if (strcmp(lexeme, ":=") == 0)
    return TOKEN_ASSIGNMENT;
  if (strcmp(lexeme, ";") == 0)
    return TOKEN_SEMICOLON;
  if (strcmp(lexeme, "+") == 0)
    return TOKEN_PLUS;
  if (strcmp(lexeme, "-") == 0)
    return TOKEN_MINUS;
  if (isdigit(lexeme[0]))
    return TOKEN_NUMBER;
  if (isalpha(lexeme[0]))
    return TOKEN_VARIABLE;
  return TOKEN_UNKNOWN;
}

TOKEN_T **lex_file(FILE *fptr) {
  // file buffer
  long token_count = 0;
  long token_arr_size = 2000;
  TOKEN_T **token_arr = (TOKEN_T **)malloc(sizeof(TOKEN_T *) * token_arr_size);
  char fbuffer[2048];
  int line_no = 1;
  while (fgets(fbuffer, 2048, fptr)) {
    int col_no = 1;
    char *tok =
        strtok(fbuffer, " \t\n"); // split by space, tab and new line characters

    // parse tokens in the line
    while (tok != NULL) {

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

      token_arr[token_count++] = make_token(type, line_no, col_no, tok);
      col_no += strlen(tok) + 1;
      tok = strtok(NULL, " \t\n");
    }
    line_no++;
  }
  token_arr[token_count++] = make_token(TOKEN_EOF, line_no, 0, "EOF");
  return token_arr;
}
