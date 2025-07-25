#pragma once
#include "token.h"
#include "tokenstream.h"
#include <stdio.h>

TokenStream *lex_file(FILE *fptr);
enum TOKEN_TYPE identify_token(const char *lexeme);
