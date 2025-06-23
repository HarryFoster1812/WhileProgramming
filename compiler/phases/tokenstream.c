#pragma once
#include "tokenstream.h"
#include "util.c"

// return the current token
TOKEN_T* peek_token(TokenStream* stream) {
    return stream->tokens[stream->index];
}

// move to the next token
TOKEN_T* next_token(TokenStream* stream) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token == NULL)
        return NULL;   // if current token is NULL, end of stream
    (stream->index)++; // move to next position
    return current_token;
}

// returns if the token exists and it matches the given type
int match(TokenStream* stream, enum TOKEN_TYPE type) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type == type) {
        // consume the token and return true
        next_token(stream);
        return 1;
    }
    return 0;
}

// consumes and returns the token, outputs an error otherwise
TOKEN_T* expect(TokenStream* stream, enum TOKEN_TYPE type) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type != type)
        expected_token(current_token->line_number, current_token->col_number,
                       current_token->type, type);
    return next_token(stream);
}

int peek_token_is(TokenStream* stream, enum TOKEN_TYPE type) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type == type)
        return 1;
    return 0;
}

int eof(TokenStream* stream) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type == TOKEN_EOF)
        return 1;
    return 0;
}

void free_tokenstream(TokenStream* stream) {
    for (size_t i = 0; i < stream->index; ++i) {
        TOKEN_T* current = stream->tokens[i];
        free(current->lexeme); // free the lexmes strings
        free(current);         // free the token struct
    }
    free(stream->tokens); // free the pointer array
    free(stream);         // free the stream struct
}
