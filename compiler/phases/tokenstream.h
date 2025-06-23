#pragma once
#include "lexer.h"

typedef TOKEN_T **TokenStream; // array of token pointers

// return the current token
TOKEN_T *peek_token(TokenStream stream);

// move to the next token
TOKEN_T *next_token(TokenStream *stream);

// returns if the token exists and it matches the given type
int match(TokenStream *stream, TOKEN_TYPE type);

// consumes and returns the token, outputs an error otherwise
TOKEN_T *expect(TokenStream *stream, TOKEN_TYPE type);

// consumes and returns the token, outputs an error otherwise
int peek_token_is(TokenStream stream, TOKEN_TYPE type);

int eof(TokenStream stream);

#include "tokenstream.c"
