#pragma once
#include "token.h"
#include <stddef.h>

typedef struct {
  TOKEN_T **tokens; // pointer to array of TOKEN_T*
  size_t index;     // current index
  size_t length;    // total number of tokens
} TokenStream;

// return the current token

TOKEN_T *peek_token(TokenStream *stream);

// move to the next token
TOKEN_T *next_token(TokenStream *stream);

// returns if the token exists and it matches the given type
int match(TokenStream *stream, enum TOKEN_TYPE type);

// consumes and returns the token, outputs an error otherwise
TOKEN_T *expect(TokenStream *stream, enum TOKEN_TYPE type);

// consumes and returns the token, outputs an error otherwise
int peek_token_is(TokenStream *stream, enum TOKEN_TYPE type);

int eof(TokenStream *stream);

void free_tokenstream(TokenStream *stream);

#include "tokenstream.c"
