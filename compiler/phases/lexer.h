#pragma once
#include "token.h"
#include "tokenstream.h"
#include <stdio.h>

TokenStream *lex_file(FILE *fptr);

#include "./lexer.c"

// TODO:
// Add comment support (use c style syntax //)
