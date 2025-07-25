#pragma once

#include "token.h" // For enum TOKEN_TYPE
#include <stdio.h>

// Panic function: prints error message and exits
void panic(const char *message);

// Converts a TOKEN_TYPE enum to a string
const char *token_type_str(enum TOKEN_TYPE token);

// Reports an expected token error
void expected_token(int line, int column, enum TOKEN_TYPE received,
                    enum TOKEN_TYPE expected);

// Reports an unexpected token error
void unexpected_token(int line, int column, const char *token);

// Reports an error for expected comparison operator token
void expected_comp(int line, int column, const char *token);

// Reports a generic parse failure
void failed_parse();

// A global flag to indicate if parsing error occurred
extern int parsing_error;
