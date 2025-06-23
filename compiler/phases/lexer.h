#pragma once
#include "token.h"
#include "tokenstream.h"
#include <stdio.h>

TOKEN_T *make_token(enum TOKEN_TYPE type, int line, int col, char *lexeme);

TokenStream *lex_file(FILE *fptr);

#include "./lexer.c"

// TODO:
// Add comment support (use c style syntax //)
